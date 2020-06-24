#include "preferences.hpp"
#include <fstream>
#include <string>
#include <map>

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

void filterPreferencesMap(std::map<std::string, std::string> &preferencesMap) {
    if(preferencesMap.find("window_width") != preferencesMap.end()) {
        if(!hasOnlyDigits(preferencesMap["window_width"])) {
            preferencesMap.erase("window_width");
            std::cout << "window_width value is invalid\n";
        }
    }
    if(preferencesMap.find("window_height") != preferencesMap.end()) {
        if(!hasOnlyDigits(preferencesMap["window_height"])) {
            preferencesMap.erase("window_height");
            std::cout << "window_height value is invalid\n";
        }
    }
    if(preferencesMap.find("full_line_highlight") != preferencesMap.end()) {
        if(!hasOnlyDigits(preferencesMap["full_line_highlight"])) {
            preferencesMap.erase("full_line_highlight");
            std::cout << "full_line_highlight value is invalid\n";
        }
    }
    if(preferencesMap.find("cursor_color") != preferencesMap.end()) {
        std::string cursorColor = preferencesMap["cursor_color"];
        if(!hasOnlyHexSymbols(cursorColor) || cursorColor.length() != 7 || cursorColor[0] != '#') {
            preferencesMap.erase("cursor_color");
            std::cout << "cursor_color value is invalid\n";
        }
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