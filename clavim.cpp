#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

const int FONT_HEIGHT = 24;
int windowWidth = 640;
int windowHeight = 480;
int FONT_WIDTH = NULL;                   // Width of a single character of the font, defined later
int currentLine = 0;                     // The line the user is currently on
int currentColumn = 0;                   // The column the user is current on
int currentTopLine = 0;                  // The line which is rendered at the top of the text
bool hasUnsavedChanges = false;          // Represents if the user has made changes to the file and has not saved
bool isSelectingAll = false;             // Represents if the user is selecting all due to shortcut
std::fstream file;                       // The file to be read and write
std::string saveFilePath;                // Place to read and then write
std::string windowTitle = "clavim";      // Title of SDL2 window
std::vector<std::string> text = {""};    // Stores text of file, each string elements represents one line

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
SDL_Surface* gTextSurface = NULL;
SDL_Texture* gTextTexture = NULL;
SDL_Color TEXT_COLOR = {255, 255, 255};

/*
Initialize related to SDL2
*/
void init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    gFont = TTF_OpenFont("SourceCodePro-Regular.ttf", FONT_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    char singleCharacter[] = " "; // A single character to measure font width (monospace)
    int width, height;            // Height is not actually read later on
    TTF_SizeText(gFont, singleCharacter, &width, &height);
    FONT_WIDTH = width;
}

/*
Close related to SDL2
*/
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

/*
Input existing content of file into text vector
*/
void InputFileToText(std::fstream& file) {
    file.open(saveFilePath.c_str(), std::ios::in); // Open in read mode
    std::string line;
    int lineNumber = 0;
    while(std::getline(file, line)) {
        if(lineNumber > text.size() - 1) text.push_back("");
        text[lineNumber] = line;
        lineNumber++;
    }
    file.close();
}

/*
Write the content of text vector into the file
*/
void SaveTextToFile(std::fstream& file) {
    file.open(saveFilePath.c_str(), std::ios::out);
    for(int i = 0; i < text.size(); i++) {
        file << text[i] << "\n";
    }
    file.close();
}

/*
Given a line the user is working one and the line which appears at the top, highlights the line an alternate color to the background
*/
void RenderLineHighlight(int currentLine, int currentTopLine) {
    SDL_Rect highlight;
    if(!isSelectingAll) {
        highlight = {0, (currentLine * FONT_HEIGHT) - (currentTopLine * FONT_HEIGHT), windowWidth, FONT_HEIGHT};
    } else {
        highlight = {0, 0, windowWidth, windowHeight};
    }
    SDL_SetRenderDrawColor(gRenderer, 0x21, 0x21, 0x21, 0xFF);
    SDL_RenderFillRect(gRenderer, &highlight);
    SDL_RenderDrawRect(gRenderer, &highlight);
}

/*
Renders text
*/
void RenderText(std::vector<std::string> text, int currentTopLine) {
    for(int i = currentTopLine; i < text.size(); i++) {
        std::string line = std::string(((std::to_string(text.size())).size()) - (std::to_string(i + 1).size()), ' ') + std::to_string(i + 1) + ' ' +  text[i];
        gTextSurface = TTF_RenderText_Solid(gFont, line.c_str(), TEXT_COLOR);
        gTextTexture = SDL_CreateTextureFromSurface(gRenderer, gTextSurface);
        int width, height; // Height is not actually read later on
        TTF_SizeText(gFont, line.c_str(), &width, &height); // Use actual text size instead of FONT_WIDTH for extra precision
        SDL_Rect textRect = {0, ((i * FONT_HEIGHT) - (currentTopLine * FONT_HEIGHT)), width, FONT_HEIGHT};
        SDL_RenderCopy(gRenderer, gTextTexture, NULL, &textRect);
        SDL_FreeSurface(gTextSurface);
        SDL_DestroyTexture(gTextTexture);
    }
}

/*
Render cursor
*/
void RenderCursor(int currentLine, int currentColumn, int currentTopLine) {
    SDL_Rect cursor = {
        (FONT_WIDTH * ((std::to_string(text.size())).size())) + FONT_WIDTH + (currentColumn * FONT_WIDTH),
        (currentLine * FONT_HEIGHT) - (currentTopLine * FONT_HEIGHT),
        FONT_WIDTH,
        FONT_HEIGHT
    };
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
    if(currentColumn >= text[currentLine].length()) {
        SDL_RenderFillRect(gRenderer, &cursor);   
    }
    SDL_RenderDrawRect(gRenderer, &cursor);
}

/*
Returns if the currentLine value will mean the line would be out of the screen
*/
bool ShouldIncreaseCurrentTopLine(int currentTopLine, int currentLine) {
    return ((currentLine * FONT_HEIGHT) - (currentTopLine * FONT_HEIGHT) >= windowHeight);
}

/*
Returns if the currentLine value will mean the line would be out of the screen
*/
bool ShouldDecreaseCurrentTopLine(int currentTopLine, int currentLine) {
    return ((currentLine * FONT_HEIGHT) - (currentTopLine * FONT_HEIGHT) < 0);
}

int main(int argc, char* args[]) {
    saveFilePath = args[1];
    file.open(saveFilePath.c_str());
    if(file) {
        file.close();
        init();

        bool quit = false;
        SDL_Event event;

        InputFileToText(file);

        windowTitle += (" - " + saveFilePath);
        SDL_SetWindowTitle(gWindow, windowTitle.c_str());

        while(!quit) {
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                } else if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_UP) { // User wants to navigate to the line above
                        if(currentLine > 0) {
                            currentLine--;
                            if(ShouldDecreaseCurrentTopLine(currentTopLine, currentLine)) currentTopLine--;
                        }
                    } else if(event.key.keysym.sym == SDLK_DOWN) { // User wants to navigate the line below
                        if(currentLine + 1 < text.size()) {
                            currentColumn = std::min(static_cast<int>(text[currentLine + 1].length()), currentColumn);
                            currentLine += 1;
                            if(ShouldIncreaseCurrentTopLine(currentTopLine, currentLine)) currentTopLine++;
                        }
                    } else if(event.key.keysym.sym == SDLK_BACKSPACE) { // User wants to either delete character on line or the line itself
                        hasUnsavedChanges = true;
                        if(isSelectingAll) { // If the user is selecting entire text
                            currentColumn = 0;
                            currentLine = 0;
                            text.clear();
                            text.push_back("");
                        } else {
                            if(text[currentLine].length()) { // If there is at least one character in the line
                                if(currentColumn) { // If there is a character before the cursor (which is to be deleted)
                                    text[currentLine].erase(text[currentLine].begin() + currentColumn - 1);
                                    currentColumn--;
                                }
                            } else if(text.size() > 1){ // If there is at least 2 lines, so the line before can be deleted
                                text.erase(text.begin() + currentLine);
                                currentLine = currentLine -1 >= 0 ? currentLine - 1 : 0;
                                if(ShouldDecreaseCurrentTopLine(currentTopLine, currentLine)) currentTopLine--;
                            }
                        }
                    } else if(event.key.keysym.sym == SDLK_LEFT) { // User wants to navigate left
                        currentColumn = currentColumn - 1 >= 0 ? currentColumn - 1: 0;
                    } else if(event.key.keysym.sym == SDLK_RIGHT) { // User wants to navigate right
                        currentColumn = currentColumn < text[currentLine].length() ? currentColumn + 1 : currentColumn;
                    } else if(event.key.keysym.sym == SDLK_RETURN) { // User wants to create a new line
                        hasUnsavedChanges = true;
                        text.insert(text.begin() + currentLine + 1, "");
                        currentLine++;
                        currentColumn = 0;
                        if(ShouldIncreaseCurrentTopLine(currentTopLine, currentLine)) currentTopLine++;
                    }
                    /* SHORTCUTS */
                    else if(event.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL) { // User wants to save file
                        if(hasUnsavedChanges) {
                            hasUnsavedChanges = false;
                            SaveTextToFile(file);
                            SDL_SetWindowTitle(gWindow, windowTitle.c_str());
                        }
                    } else if(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL) { // User wants to select all text
                        isSelectingAll = true;
                    }

                    if(!(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL)) {
                        isSelectingAll = false;
                    }
                } else if(event.type == SDL_TEXTINPUT) {
                    if(!(SDL_GetModState() & KMOD_CTRL)) {
                        hasUnsavedChanges = true;
                        text[currentLine].insert(currentColumn, event.text.text);
                        currentColumn++;
                    }
                } else if(event.type == SDL_WINDOWEVENT) {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        windowWidth = event.window.data1;
                        windowHeight = event.window.data2;
                    }
                }
            }

            if(hasUnsavedChanges) {
                SDL_SetWindowTitle(gWindow, (windowTitle + '*').c_str());
            }

            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(gRenderer);

            RenderLineHighlight(currentLine, currentTopLine);
            RenderText(text, currentTopLine);
            RenderCursor(currentLine, currentColumn, currentTopLine);

            SDL_RenderPresent(gRenderer);

        }

        close();
    } else {
        std::cout << "File not valid, please specify a file to edit\n";
    }
    return 0;

}