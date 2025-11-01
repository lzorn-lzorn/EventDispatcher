
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "Observer.hpp"

class Publisher {
public:
	/*
	 * @brief Subscribe an observer to an event
	 */
	void Subscribe(const std::string& event_name, const Observer* observer) {
		observers[event_name].push_back(observer);
	}

	void Unsubscribe(const std::string& event_name, const Observer* observer) {
		auto it = observers.find(event_name);
		if (it != observers.end()) {
			auto& vec = it->second;
			vec.erase(std::remove(vec.begin(), vec.end(), observer), vec.end());
		}
	}

	void Notify(const std::string& event_name){
		auto it = observers.find(event_name);
		if (it != observers.end()) {
			for (const Observer* observer : it->second) {
				if (observer) {
					observer->Execute(event_name);
				}
			}
		}
	}
private:
	static std::unordered_map<std::string, std::vector<const Observer*>> observers;
};


 