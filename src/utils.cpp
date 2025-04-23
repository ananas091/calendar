#include "utils.h"

#include <sstream>

std::unordered_map<std::string, std::string> ParseQuery(const std::string& query) {
    std::unordered_map<std::string, std::string> result;
    std::istringstream stream(query);
    std::string pair;
    while (std::getline(stream, pair, '&')) {
        auto pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            result[key] = value;
        }
    }
    return result;
}