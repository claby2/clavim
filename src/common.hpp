#ifndef COMMON_HPP
#define COMMON_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;
extern SDL_Surface* gTextSurface;
extern SDL_Texture* gTextTexture;
extern SDL_Color TEXT_COLOR;
extern SDL_Color CURSOR_COLOR;
extern SDL_Color LINE_HIGHLIGHT_COLOR;

extern int windowWidth;
extern int windowHeight;
extern int fontWidth;
extern int fontHeight;
extern int spacesPerTab;
extern std::fstream file;
extern std::string saveFilePath;
extern std::string windowTitle;
extern std::string fontName;
extern bool fullLineHighlight;

#endif