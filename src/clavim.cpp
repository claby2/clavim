#define SDL_MAIN_HANDLED

#include "preferences.hpp"
#include "common.hpp"
#include "text_area.hpp"
#include "cursor.hpp"
#include "cross_platform.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

void setFromPreferences(std::string preferencesFilePath) {
    std::map<std::string, std::string> preferencesMap = getPreferencesMap(preferencesFilePath);
    filterPreferencesMap(preferencesMap, (exePath() + dirSep));
    if(preferencesMap.find("window_width")         != preferencesMap.end()) windowWidth       = std::stoi(preferencesMap["window_width"]       );
    if(preferencesMap.find("window_height")        != preferencesMap.end()) windowHeight      = std::stoi(preferencesMap["window_height"]      );
    if(preferencesMap.find("full_line_highlight")  != preferencesMap.end()) fullLineHighlight = std::stoi(preferencesMap["full_line_highlight"]);
    if(preferencesMap.find("line_highlight_color") != preferencesMap.end()) {
        LINE_HIGHLIGHT_COLOR = {
            (uint8_t)std::stoi(preferencesMap["line_highlight_color"].substr(1, 2), nullptr, 16),
            (uint8_t)std::stoi(preferencesMap["line_highlight_color"].substr(3, 2), nullptr, 16),
            (uint8_t)std::stoi(preferencesMap["line_highlight_color"].substr(5, 2), nullptr, 16)
        };
    }
    if(preferencesMap.find("font")                 != preferencesMap.end()) fontName = preferencesMap["font"];
    if(preferencesMap.find("font_size")            != preferencesMap.end()) fontHeight = std::stoi(preferencesMap["font_size"]);
    if(preferencesMap.find("spaces_per_tab")       != preferencesMap.end()) spacesPerTab = std::stoi(preferencesMap["spaces_per_tab"]);
    if(preferencesMap.find("cursor_color")         != preferencesMap.end()) {
        CURSOR_COLOR = {
            (uint8_t)std::stoi(preferencesMap["cursor_color"].substr(1, 2), nullptr, 16),
            (uint8_t)std::stoi(preferencesMap["cursor_color"].substr(3, 2), nullptr, 16),
            (uint8_t)std::stoi(preferencesMap["cursor_color"].substr(5, 2), nullptr, 16)
        };
    }
}

std::string pwd() {
    char buffer[PATH_MAX];
    _getcwd(buffer, PATH_MAX);
    return std::string(buffer);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    gFont = TTF_OpenFont((exePath() + dirSep + fontName).c_str(), fontHeight);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    char singleCharacter[] = " "; // A single character to measure font width (monospace)
    int width, height;
    TTF_SizeText(gFont, singleCharacter, &width, &height);
    fontWidth = width;
    fontHeight = height; // Redefine height
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    gFont = NULL;
    TTF_CloseFont(gFont);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    setFromPreferences(exePath() + dirSep + "preferences.ini");

    if(args[1]) {
        saveFilePath = pwd() + dirSep + args[1];
        file.open(saveFilePath.c_str());
        if(!file) { 
            std::ofstream file {saveFilePath};
            std::cout << "\"" << args[1] << "\" [New File] has been created\n";
        }
        file.close();
        init();
        bool quit = false;
        SDL_Event event;
        TextArea textArea;
        windowTitle += (" - " + saveFilePath);
        SDL_SetWindowTitle(gWindow, windowTitle.c_str());

        while(!quit && SDL_WaitEvent(&event)) {
            textArea.setPrevious();

            if(event.type == SDL_QUIT) {
                quit = true;
            } else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_UP) { // User wants to navigate to the line above
                    textArea.up();
                } else if(event.key.keysym.sym == SDLK_DOWN) { // User wants to navigate the line below
                    textArea.down();
                } else if(event.key.keysym.sym == SDLK_BACKSPACE) { // User wants to either delete character on line or the line itself
                    textArea.backspace();
                } else if(event.key.keysym.sym == SDLK_LEFT) { // User wants to navigate left
                    textArea.left();
                } else if(event.key.keysym.sym == SDLK_RIGHT) { // User wants to navigate right
                    textArea.right();
                } else if(event.key.keysym.sym == SDLK_RETURN) { // User wants to create a new line
                    textArea.newLine();
                } else if(event.key.keysym.sym == SDLK_TAB) {
                    textArea.inputText(std::string(spacesPerTab, ' '));
                }
                /* SHORTCUTS */
                else if(event.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL) { // User wants to save file
                    textArea.save();
                } else if(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL) { // User wants to select all text
                    textArea.selectAll();
                }

                if(!(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL)) {
                    textArea.removeSelection();
                }
            } else if(event.type == SDL_TEXTINPUT) {
                if(!(SDL_GetModState() & KMOD_CTRL)) {
                    textArea.inputText(event.text.text);
                }
            } else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    textArea.resizeWindow(event.window.data1, event.window.data2);
                }
            }

            textArea.render();
        }

        close();
    } else {
        std::cout << "Please specify a file to edit\n";
    }
    return 0;

}