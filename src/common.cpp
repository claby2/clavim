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
SDL_Color CURSOR_COLOR = {255, 255, 0};             // Default cursor color if not specified in preferences.ini
SDL_Color LINE_HIGHLIGHT_COLOR = {33, 33, 33};      // Default line highlight color if not specified in preferences.ini

int windowWidth = 640;                              // Default width of window if not specified in preferences.ini
int windowHeight = 480;                             // Default height of window if not specified in preferences.ini
int fontWidth;                                      // Width of a single character of the font, defined later
int fontHeight = 16;                                // Height of a single character of the font, redefined later
int spacesPerTab = 4;                               // Default number of spaces created per tab if not specified in preferences.ini
std::fstream file;                                  // The file to be read and write
std::string saveFilePath;                           // Place to read and then write
std::string windowTitle = "clavim";                 // Title of SDL2 window
std::string fontName = "SourceCodePro-Regular.ttf"; // Default font if not specified in preferences.ini
std::string lineNumberMode = "absolute";            // Default line number mode if not specified in preferences.ini (options are: absolute, relative, hybrid)
bool fullLineHighlight = false;                     // Default representation of if the entire line should be highlighted if not specified in preferences.ini