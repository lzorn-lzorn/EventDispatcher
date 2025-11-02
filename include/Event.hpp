#include <cstdint>
#include <atomic>
#include <memory>
#include <optional>
#include <ratio>
#include <tuple>
#include <functional>
#include <type_traits>
#include <utility>
#include <string>

#include "Utilities.hpp"

struct EventHandle: NonCopyMove<EventHandle> 
{
public:
	enum class GenerateType : uint8_t 
	{
		Empty = 0,
		Valid = 1
	};
	explicit EventHandle()
	 : id(0)
	{
		
	}
	explicit EventHandle(GenerateType type)
	 : id(type == GenerateType::Valid ? GenerateNextId() : 0)
	{
		
	}

	bool operator==(const EventHandle& other) const noexcept 
	{
		return id == other.id;
	}

	bool operator!=(const EventHandle& other) const noexcept 
	{
		return id != other.id;
	}

	~EventHandle() = default;
public:
	bool IsValid() const noexcept 
	{
		return id != 0;
	}	

	void Reset() noexcept 
	{
		id = 0;
	}
private:
	static uint64_t GenerateNextId() noexcept
	{
		static std::atomic<uint64_t> id {0};
		uint64_t retNextId = ++id;

		/* 防止溢出 */
		if (retNextId == 0) {
			retNextId = ++id;
		}
		return retNextId;
	}
	uint64_t id;
};

template <typename RetVal, typename... Args>
struct EventAction 
{
	using Params = std::tuple<std::decay_t<Args>...>;
	using RetTp  = RetVal;
	using Action = std::function<RetTp(Args...)>;

	explicit EventAction(Args ... args)
	 : params(std::make_tuple(std::forward<Args>(args)...))
	{
		
	}
	
	uint8_t GetParamsNum() const noexcept 
	{
		return sizeof...(Args);
	}

	template <uint8_t Index>
	constexpr auto GetParamAt() const noexcept
		-> const std::tuple_element_t<Index, Params>&
	{
		static_assert(Index < sizeof...(Args), "Index out of bounds");
		return std::get<Index>(params);
	}

	template <uint8_t Index>
	constexpr auto GetParamAt() noexcept
		-> std::tuple_element_t<Index, Params>&
	{
		static_assert(Index < sizeof...(Args), "Index out of bounds");
		return std::get<Index>(params);
	}
	
	void Bind(Action action) noexcept
	{
		this->action = action;
	}

	bool IsValid() const noexcept {
		return bool(action);
	}
    std::optional<RetTp> Execute() {
        if (!IsValid()) 
            return std::nullopt;
    

        if constexpr (std::is_void_v<RetTp>) 
		{// 非 const 版本允许移动 params（避免不必要拷贝）
            std::apply(action, std::move(params));
        } else {
            return std::apply(action, std::move(params));
        }
    }

    std::optional<RetTp> Execute() const 
	{
        if (!IsValid()) 
            return std::nullopt;
    
        if constexpr (std::is_void_v<RetTp>) 
		{// const 版本使用 const 引用，不会移动 members
            std::apply(action, params);
        } else {
            return std::apply(action, params);
        }
    }

private:
	Params params;
	Action action;
};

template <typename RetVal, typename... Args>
class Event
{
	using Action = EventAction<RetVal, Args...>; 
	using Handle = EventHandle;
	using Params = Action::Params;
	using RetTp  = Action::RetTp;
	using FnTp   = Action::Action;
	
public:
	explicit Event() : handle(EventHandle::GenerateType::Valid)
	{
	}

	void BindEventAction(FnTp func, Args... args)
	{
		if (handle.IsValid()){
			action = Action(std::forward<Args>(args)...);
			action.Bind(func);
		}
		
	}

	std::optional<RetTp> Execute() const {
		return action.Execute();
	}

	std::optional<RetTp> Execute() {
		return action.Execute();
	}

	Action GetEventAction() const {
		return action;
	}
private:
	std::string name;
	Handle handle;
	Action action;
};