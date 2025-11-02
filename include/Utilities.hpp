#pragma once
#include <utility>
#include <tuple>
#include <functional>
#include <type_traits>

// 删除拷贝操作（派生类会自动变为不可拷贝）
template<typename Tag = void>
struct NonCopy {
protected:
    NonCopy() = default;
    ~NonCopy() = default;

    NonCopy(const NonCopy&) = delete;
    NonCopy& operator=(const NonCopy&) = delete;
};

// 删除移动操作（派生类会自动变为不可移动）
template<typename Tag = void>
struct NonMove {
protected:
    NonMove() = default;
    ~NonMove() = default;

    NonMove(NonMove&&) = delete;
    NonMove& operator=(NonMove&&) = delete;
};

// 同时删除拷贝与移动（可通过 Tag 区分相同基类的多重继承）
template<typename Tag = void>
struct NonCopyMove : NonCopy<Tag>, NonMove<Tag> {
protected:
    NonCopyMove() = default;
    ~NonCopyMove() = default;
};


template <typename Func, typename Tuple, std::size_t... I>
decltype(auto) ApplyWithUnpackedTupleImpl(Func&& func, Tuple&& tuple, std::index_sequence<I...>) {
    return std::forward<Func>(func)(std::get<I>(std::forward<Tuple>(tuple))...);
}

/*
 * @brief: 使用元组解包调用函数, 用C++17之前没有std::apply的环境
 * @Usage: sum =  ApplyWithUnpackedTuple(
 *             [](int a, int b){ return a + b; }, 
 *             std::make_tuple(1, 2)
 *         ); // sum == 3
 */
template <typename Func, typename Tuple>
decltype(auto) ApplyWithUnpackedTuple(Func&& func, Tuple&& tuple) {
    return ApplyWithUnpackedTupleImpl(
        std::forward<Func>(func),
        std::forward<Tuple>(tuple),
        std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
    );
}