#ifndef CLAVIM_HPP
#define CLAVIM_HPP

#include <string>

/*
Set variables based on preferences
*/
void setFromPreferences(std::string preferencesFilePath);

/*
Get current working directory
*/
std::string pwd();

/*
Initialize related to SDL2
*/
void init();

/*
Close related to SDL2
*/
void close();

int main(int argc, char* args[]);

#endif