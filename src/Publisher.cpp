#include "../include/Publisher.hpp"

// Define the static member
std::unordered_map<std::string, std::vector<const Observer*>> Publisher::observers;