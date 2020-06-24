#include "preferences.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>

std::fstream preferences;

std::string getFilteredString(std::string s) {
    std::string output;
    output.reserve(s.size());
    for(size_t i = 0; i < s.size(); i++) {
        if(s[i] != ' ' && s[i] != '\"') output += s[i];
    }
    return output;
}

bool hasOnlyDigits(std::string s) {
    return s.find_first_not_of("0123456789") == std::string::npos;
}

bool hasOnlyHexSymbols(std::string s) {
    return s.find_first_not_of("#0123456789ABCDEFabcdef") == std::string::npos;
}

bool eraseIfNotValidNumber(std::map<std::string, std::string> &preferencesMap, std::string key) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        if(!hasOnlyDigits(preferencesMap[key])) {
            preferencesMap.erase(key);
            return true;
        }
    }
    return false;
}

bool eraseIfNotValidColor(std::map<std::string, std::string> &preferencesMap, std::string key) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        std::string cursorColor = preferencesMap[key];
        if(!hasOnlyHexSymbols(cursorColor) || cursorColor.length() != 7 || cursorColor[0] != '#') {
            preferencesMap.erase(key);
            return true;
        }
    }
    return false;
}

void filterPreferencesMap(std::map<std::string, std::string> &preferencesMap) {
    std::vector<std::string> invalidPropertyNames;
    if(eraseIfNotValidNumber(preferencesMap, "window_width")) invalidPropertyNames.push_back("window_width");
    if(eraseIfNotValidNumber(preferencesMap, "window_height")) invalidPropertyNames.push_back("window_height");
    if(eraseIfNotValidNumber(preferencesMap, "full_line_highlight")) invalidPropertyNames.push_back("full_line_highlight");
    if(eraseIfNotValidColor(preferencesMap, "cursor_color")) invalidPropertyNames.push_back("cursor_color");
    if(eraseIfNotValidColor(preferencesMap, "line_highlight_color")) invalidPropertyNames.push_back("line_highlight_color");
    if(invalidPropertyNames.size() > 0) {
        std::cout << "WARN: The following preference(s) have not been properly defined in preferences.ini:\n\n";
        for(int i = 0; i < invalidPropertyNames.size(); i++) {
            std::cout << "\t- " << invalidPropertyNames[i] << "\n";
        }
        std::cout << "\nThe default values for these properties have been used instead.\n";
    }
}

std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath) {
    std::map<std::string, std::string> preferencesMap;
    preferences.open(preferencesFilePath.c_str(), std::ios::in);
    std::string line;
    while(std::getline(preferences, line)) {
        if(line[0] != ';' && line[0] != '#' && line[0] != '[' && !line.empty()) {
            std::size_t delimiter = line.find("=");
            std::string name = getFilteredString(line.substr(0, delimiter));
            std::string value = getFilteredString(line.substr(delimiter + 1, line.length()));
            preferencesMap[name] = value;
        }
    }
    preferences.close();
    filterPreferencesMap(preferencesMap);
    return preferencesMap;
}