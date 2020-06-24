#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP
#include <fstream>
#include <string>
#include <map>

extern std::fstream preferences;

/*
Returns map from preferences.ini
*/
std::map<std::string, std::string> getPreferencesMap(std::string preferencesFilePath);

/*
Returns string without space character(s)
*/
std::string removeSpaces(std::string str);


#endif