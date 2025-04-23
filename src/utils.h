#pragma once
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> ParseQuery(const std::string& query);