#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP
#include <fstream>
#include <string>
#include <map>

extern std::fstream preferences;

/*
Returns string without space character(s)
*/
std::string getFilteredString(std::string str);

/*
Returns if the string only has digits (0123456789)
*/
bool hasOnlyDigits(std::string s);

/*
Returns if the string only has hex symbols (#0123456789ABCDEFabcdef)
*/
bool hasOnlyHexSymbols(std::string s);

/*
Filters the preferences map, removes any elements which are invalid
*/
void filterPreferencesMap(std::map<std::string, std::string> &preferencesMap);

/*
Returns map from preferences.ini
*/
std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath);

#endif