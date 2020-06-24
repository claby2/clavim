#include "preferences.hpp"
#include <fstream>
#include <string>
#include <map>

std::fstream preferences;

std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath) {
    std::map<std::string, std::string> preferencesMap;
    preferences.open(preferencesFilePath.c_str(), std::ios::in);
    std::string line;
    while(std::getline(preferences, line)) {
        if(line[0] != ';' && line[0] != '#' && line[0] != '[' && !line.empty()) {
            std::size_t delimiter = line.find("=");
            std::string name = removeSpaces(line.substr(0, delimiter));
            std::string value = removeSpaces(line.substr(delimiter + 1, line.length()));
            preferencesMap[name] = value;
        }
    }
    preferences.close();
    return preferencesMap;
}

std::string removeSpaces(std::string str) {
    std::string output;
    output.reserve(str.size());
    for(size_t i = 0; i < str.size(); i++) {
        if(str[i] != ' ') output += str[i];
    }
    return output;
}