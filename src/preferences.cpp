#include "preferences.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>

std::fstream preferences;

Preference recognizedPreferences[] = {
    {.name = "window_width", .type = "number"},
    {.name = "window_height", .type = "number"},
    {.name = "full_line_highlight", .type = "number"},
    {.name = "font_size", .type = "number"},
    {.name = "spaces_per_tab", .type = "number"},
    {.name = "cursor_color", .type = "color"},
    {.name = "line_highlight_color", .type = "color"},
    {.name = "font", .type = "file"}
};

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
        std::string value = preferencesMap[key];
        if(!hasOnlyDigits(value) || value.empty()) {
            preferencesMap.erase(key);
            return true;
        }
    }
    return false;
}

bool eraseIfNotValidColor(std::map<std::string, std::string> &preferencesMap, std::string key) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        std::string color = preferencesMap[key];
        if(!hasOnlyHexSymbols(color) || color.length() != 6 || color.empty()) {
            preferencesMap.erase(key);
            return true;
        }
    }
    return false;
}

bool eraseIfNotValidFile(std::map<std::string, std::string> &preferencesMap, std::string key, std::string filePath) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        std::string fullFilePath = filePath + preferencesMap[key];
        if(FILE *file = fopen(fullFilePath.c_str(), "r")) {
            fclose(file);
        } else {
            preferencesMap.erase(key);
            return true;
        }
    }
    return false;
}

void filterPreferencesMap(std::map<std::string, std::string> &preferencesMap, std::string filePath) {
    std::vector<std::string> invalidPreferences;
    for(int i = 0; i < (sizeof(recognizedPreferences)/sizeof(*recognizedPreferences)) ; i++) {
        Preference preference = recognizedPreferences[i];
        if(
            (preference.type == "number" && eraseIfNotValidNumber(preferencesMap, preference.name)) ||
            (preference.type == "color"  && eraseIfNotValidColor (preferencesMap, preference.name)) ||
            (preference.type == "file"   && eraseIfNotValidFile  (preferencesMap, preference.name, filePath))) {
            invalidPreferences.push_back(preference.name);
        }   
    }
    if(invalidPreferences.size() > 0) {
        std::cout << "WARN: The following preferences have not been properly defined in preferences.ini:\n\n";
        for(int i = 0; i < invalidPreferences.size(); i++) {
            std::cout << "\t- " << invalidPreferences[i] << "\n";
        }
        std::cout << "\nThe default values for these preferences have been used instead.\n";
    }
}

std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath) {
    std::map<std::string, std::string> preferencesMap;
    preferences.open(preferencesFilePath.c_str(), std::ios::in);
    std::string line;
    while(std::getline(preferences, line)) {
        if(line[0] != ';' && line[0] != '#' && line[0] != '[' && !line.empty()) {
            line = line.substr(0, line.find("#"));
            line = line.substr(0, line.find(";"));
            std::size_t delimiter = line.find("=");
            std::string name = getFilteredString(line.substr(0, delimiter));
            std::string value = getFilteredString(line.substr(delimiter + 1, line.length()));
            preferencesMap[name] = value;
        }
    }
    preferences.close();
    return preferencesMap;
}