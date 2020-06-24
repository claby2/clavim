#include "common.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
SDL_Surface* gTextSurface = NULL;
SDL_Texture* gTextTexture = NULL;
SDL_Color TEXT_COLOR = {255, 255, 255};

int windowWidth = 640;                        // Default width of window if not specified in preferences.ini
int windowHeight = 480;                       // Default height of window if not specified in preferences.ini
int fontWidth;                                // Width of a single character of the font, defined later
int fontHeight = 16;                          // Height of a single character of the font, redefined later
std::fstream file;                            // The file to be read and write
std::string saveFilePath;                     // Place to read and then write
std::string windowTitle = "clavim";           // Title of SDL2 window
bool fullLineHighlight = true;                // Default representation of if the entire line should be highlighted or only the line number