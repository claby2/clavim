#include "preferences.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

std::fstream preferences;

Preference recognizedPreferences[] = {
    {.name = "window_width", .type = "number"},
    {.name = "window_height", .type = "number"},
    {.name = "full_line_highlight", .type = "number"},
    {.name = "font_size", .type = "number"},
    {.name = "spaces_per_tab", .type = "number"},
    {.name = "cursor_color", .type = "color"},
    {.name = "line_highlight_color", .type = "color"},
    {.name = "font", .type = "file"},
    {.name = "line_number_mode", .type = "lineNumberMode"}
};

std::string getFilteredString(std::string s) {
    std::string output;
    output.reserve(s.size());
    bool foundQuote = false; // If found an opening quote char
    for(size_t i = 0; i < s.size(); i++) {
        if(foundQuote) {
            if(s[i] == '\"') {
                foundQuote = false;
            } else {
                output += s[i];
            }
        } else {
            if(s[i] == '\"') {
                foundQuote = true;
            }
            if(!isspace(s[i]) && s[i] != '\"') output += s[i];
        }
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
        if(!hasOnlyDigits(value) || value.empty() || (key == "spaces_per_tab" && std::stoi(value) < 1)) {
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

bool eraseIfNotValidLineNumberMode(std::map<std::string, std::string> &preferencesMap, std::string key) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        std::string mode = preferencesMap[key];
        std::transform(mode.begin(), mode.end(), mode.begin(),
            [](unsigned char c){ return std::tolower(c); }); // Turn into lower case
        if(mode != "absolute" && mode != "relative" && mode != "hybrid") {
            preferencesMap.erase(key);
            return true;
        } else {
            preferencesMap[key] = mode; // In case preference is not lower case as mode is confirmed to be all lower case
        }
    }
    return false;
}

bool eraseIfNotValidFile(std::map<std::string, std::string> &preferencesMap, std::string key, std::string filePath) {
    if(preferencesMap.find(key) != preferencesMap.end()) {
        std::string fullFilePath = filePath + preferencesMap[key];
        FILE *file = fopen(fullFilePath.c_str(), "r");
        if(file) {
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
            (preference.type == "lineNumberMode" && eraseIfNotValidLineNumberMode(preferencesMap, preference.name)) ||
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
        if(!line.empty() && line[0] != ';' && line[0] != '#' && line[0] != '[') {
            std::size_t delimiter;
            delimiter = line.find("#");
            if(delimiter != std::string::npos) line = line.substr(0, delimiter);
            delimiter = line.find(";");
            if(delimiter != std::string::npos) line = line.substr(0, delimiter);
            delimiter = line.find("=");
            if(delimiter != std::string::npos) {
                std::string name = getFilteredString(line.substr(0, delimiter));
                std::string value = getFilteredString(line.substr(delimiter + 1, line.length()));
                preferencesMap[name] = value;
            }
        }
    }
    preferences.close();
    return preferencesMap;
}