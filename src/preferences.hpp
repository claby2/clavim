#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP
#include <fstream>
#include <string>
#include <map>

extern std::fstream preferences;

struct Preference {
    std::string name;
    std::string type;
};

extern Preference recognizedPreferences[];

/*
Returns string without space and double quotation character(s)
*/
std::string getFilteredString(std::string s);

/*
Returns if the string only has digits (0123456789)
*/
bool hasOnlyDigits(std::string s);

/*
Returns if the string only has hex symbols (#0123456789ABCDEFabcdef)
*/
bool hasOnlyHexSymbols(std::string s);

/*
Erases element from map if value is not a positive number, returns true if element was erased, returns false if element was not erased
*/
bool eraseIfNotValidNumber(std::map<std::string, std::string> &preferencesMap, std::string key);

/*
Erases element from map if value is not a valid color, returns true if element was erased, returns false if element was not erased
*/
bool eraseIfNotValidColor(std::map<std::string, std::string> &preferencesMap, std::string key);

/*
Erases element from map if the value is not a valid line number mode, returns true if element was erased, returns false if element was not erased
*/
bool eraseIfNotValidLineNumberMode(std::map<std::string, std::string> &preferencesMap, std::string key);

/*
Erases element from map if value is not a valid file, returns true if element was erased, returns false if element was not erased
*/
bool eraseIfNotValidFile(std::map<std::string, std::string> &preferencesMap, std::string key, std::string filePath);

/*
Filters the preferences map, removes any elements which are invalid
*/
void filterPreferencesMap(std::map<std::string, std::string> &preferencesMap, std::string filePath);

/*
Returns map from preferences.ini
*/
std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath);

#endif