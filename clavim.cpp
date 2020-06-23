#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define dirSep "\\" // Directory seperator for windows
#define PATH_MAX MAX_PATH

/*
Get exe path, windows
*/
std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
#elif __linux__
#include <unistd.h>
#include <limits.h>
#define _getcwd getcwd
#define dirSep "/" // Directory seperator

/*
Get exe path, linux
*/
std::string ExePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string path = std::string(result, (count > 0) ? count : 0);
    path = path.substr(0, path.find_last_of('/'));
    return path;
}
#else 
#if (defined (__APPLE__) && defined (__MACH__))
#include <mach-o/dyld.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <limits.h>
#define _getcwd getcwd
#define dirSep "/" // Directory seperator

/*
Get exe path, apple
*/
std::string ExePath(){
    char buf[PATH_MAX];
    uint32_t size = sizeof(buf);
    if(_NSGetExecutablePath(buf, &size) == 0) {
        std::string path = buf;
        path = path.substr(0, path.find_last_of('/'));
        return path;
    }
}
#else
    #error PLATFORM NOT SUPPORTED
#endif
#endif

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
SDL_Surface* gTextSurface = NULL;
SDL_Texture* gTextTexture = NULL;
SDL_Color TEXT_COLOR = {255, 255, 255};

int windowWidth = 640;
int windowHeight = 480;
int fontWidth;                                // Width of a single character of the font, defined later
int fontHeight = 16;                          // Height of a single character of the font, redefined later
bool hasUnsavedChanges = false;               // Represents if the user has made changes to the file and has not saved
bool isSelectingAll = false;                  // Represents if the user is selecting all due to shortcut
bool forceRender = true;                      // Force render on start up despite no changes being made
std::fstream file;                            // The file to be read and write
std::string saveFilePath;                     // Place to read and then write
std::string windowTitle = "clavim";           // Title of SDL2 window
int currentLine = 0;                          // The line the user is currently on
int currentColumn = 0;                        // The column the user is current on
int currentTopLine = 0;                       // The line which is rendered at the top of the text
int leftColumn = 0;                           // Column number of the column present at the start of the screen (without offset)
std::vector<std::string> text = {""};         // Stores text of file, each string elements represents one line
int previousLine;                             // Equal to current line at the start of the main loop to detect changes
int previousColumn;                           // Equal to current column at the start of the main loop to detect changes
std::vector<std::string> previousText = {""}; // Equal to text vector at the start of the main loop to detect changes

class Cursor {
    public:
        int offset, x, y, width, height;

        Cursor() {
            width = fontWidth;
            height = fontHeight;
        }

        void Update() {
            offset = (fontWidth * (int)((std::to_string(text.size())).size())) + fontWidth;
            x = offset + ((currentColumn - leftColumn) * fontWidth);
            y = (currentLine * fontHeight) - (currentTopLine * fontHeight);
        }

        SDL_Rect GetRect() {
            SDL_Rect cursorRect = {x, y, width, height};
            return cursorRect;
        }
};

/*
Get current working directory
*/
std::string pwd() {
    char buffer[PATH_MAX];
    _getcwd(buffer, PATH_MAX);
    return std::string(buffer);
}

/*
Initialize related to SDL2
*/
void init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    gFont = TTF_OpenFont((ExePath() + dirSep + "SourceCodePro-Regular.ttf").c_str(), fontHeight);
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
void InputFileToText() {
    file.open(saveFilePath.c_str(), std::ios::in);
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
void SaveTextToFile() {
    file.open(saveFilePath.c_str(), std::ios::out);
    for(int i = 0; i < text.size(); i++) {
        file << text[i] << "\n";
    }
    file.close();
}

/*
Given a line the user is working one and the line which appears at the top, highlights the line an alternate color to the background
*/
void RenderLineHighlight() {
    SDL_Rect highlight;
    if(!isSelectingAll) {
        highlight = {0, (currentLine * fontHeight) - (currentTopLine * fontHeight), windowWidth, fontHeight};
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
void RenderText() {
    int currentLastLine = std::min(
        (int)text.size(), 
        (int)(currentTopLine + (windowHeight / fontHeight) + 1)
    );
    for(int i = currentTopLine; i < currentLastLine; i++) {
        std::string lineContent = text[i];
        lineContent.erase(0, leftColumn);

        std::string line = std::string(((std::to_string(text.size())).size()) - (std::to_string(i + 1).size()), ' ') + std::to_string(i + 1) + ' ' +  lineContent;
        gTextSurface = TTF_RenderUTF8_Blended(gFont, line.c_str(), TEXT_COLOR);
        gTextTexture = SDL_CreateTextureFromSurface(gRenderer, gTextSurface);
        int width, height; // Height is not actually read later on
        TTF_SizeText(gFont, line.c_str(), &width, &height); // Use actual text size instead of fontWidth for extra precision
        SDL_Rect textRect = {0, ((i * fontHeight) - (currentTopLine * fontHeight)), width, fontHeight};
        SDL_RenderCopy(gRenderer, gTextTexture, NULL, &textRect);
        SDL_FreeSurface(gTextSurface);
        SDL_DestroyTexture(gTextTexture);

        if(width > windowWidth) {
            SDL_Rect endLineHighlight = {
                windowWidth - fontWidth,
                ((i * fontHeight) - (currentTopLine * fontHeight)),
                fontWidth,
                fontHeight
            };
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(gRenderer, &endLineHighlight);
        }
    }
}

/*
Render cursor
*/
void RenderCursor(Cursor &cursor) {
    cursor.Update();
    SDL_Rect cursorRect = cursor.GetRect();
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
    if(currentColumn >= text[currentLine].length()) {
        SDL_RenderFillRect(gRenderer, &cursorRect);   
    }
    SDL_RenderDrawRect(gRenderer, &cursorRect);
}

/*
Returns if the currentLine value will mean the line would be out of the screen
*/
bool ShouldIncreaseCurrentTopLine() {
    return ((currentLine * fontHeight) - (currentTopLine * fontHeight) >= windowHeight);
}

/*
Returns if the currentLine value will mean the line would be out of the screen
*/
bool ShouldDecreaseCurrentTopLine() {
    return ((currentLine * fontHeight) - (currentTopLine * fontHeight) < 0);
}

int main(int argc, char* args[]) {
    if(args[1]) saveFilePath = pwd() + dirSep + args[1];
    file.open(saveFilePath.c_str());
    if(file) {
        file.close();
        init();

        bool quit = false;
        SDL_Event event;

        Cursor cursor;

        InputFileToText();

        windowTitle += (" - " + saveFilePath);
        SDL_SetWindowTitle(gWindow, windowTitle.c_str());

        cursor.Update();
        while(!quit) {
            previousLine = currentLine;
            previousColumn = currentColumn;
            previousText = text;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                } else if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_UP) { // User wants to navigate to the line above
                        if(currentLine > 0) {
                            currentColumn = std::min(static_cast<int>(text[currentLine - 1].length()), currentColumn);
                            currentLine--;
                            if(ShouldDecreaseCurrentTopLine()) currentTopLine--;
                        }
                    } else if(event.key.keysym.sym == SDLK_DOWN) { // User wants to navigate the line below
                        if(currentLine + 1 < text.size()) {
                            currentColumn = std::min(static_cast<int>(text[currentLine + 1].length()), currentColumn);
                            currentLine += 1;
                            if(ShouldIncreaseCurrentTopLine()) currentTopLine++;
                        }
                    } else if(event.key.keysym.sym == SDLK_BACKSPACE) { // User wants to either delete character on line or the line itself
                        hasUnsavedChanges = true;
                        if(isSelectingAll) { // If the user is selecting entire text
                            currentColumn = 0;
                            currentLine = 0;
                            text.clear();
                            text.push_back("");
                            leftColumn = 0;
                        } else {
                            if(text[currentLine].length()) { // If there is at least one character in the line
                                if(currentColumn) { // If there is a character before the cursor (which is to be deleted)
                                    text[currentLine].erase(text[currentLine].begin() + currentColumn - 1);
                                    currentColumn--;
                                    leftColumn = std::min(currentColumn, leftColumn);
                                }
                            } else if(text.size() > 1){ // If there is at least 2 lines, so the line before can be deleted
                                text.erase(text.begin() + currentLine);
                                currentLine = currentLine -1 >= 0 ? currentLine - 1 : 0;
                                if(ShouldDecreaseCurrentTopLine()) currentTopLine--;
                            }
                        }
                    } else if(event.key.keysym.sym == SDLK_LEFT) { // User wants to navigate left
                    if(currentColumn - 1 >= 0) {
                        currentColumn--;
                        leftColumn = std::min(currentColumn, leftColumn);
                    }
                    } else if(event.key.keysym.sym == SDLK_RIGHT) { // User wants to navigate right
                        if(currentColumn < text[currentLine].length()) {
                            currentColumn++;
                            cursor.Update();
                            if(cursor.x >= windowWidth) {
                                leftColumn++;
                            }
                        }
                    } else if(event.key.keysym.sym == SDLK_RETURN) { // User wants to create a new line
                        hasUnsavedChanges = true;
                        text.insert(text.begin() + currentLine + 1, "");
                        currentLine++;
                        currentColumn = 0;
                        if(ShouldIncreaseCurrentTopLine()) currentTopLine++;
                    }
                    /* SHORTCUTS */
                    else if(event.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL) { // User wants to save file
                        if(hasUnsavedChanges) {
                            hasUnsavedChanges = false;
                            SaveTextToFile();
                            SDL_SetWindowTitle(gWindow, windowTitle.c_str());
                        }
                    } else if(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL) { // User wants to select all text
                        isSelectingAll = true;
                        forceRender = true;
                    }

                    if(!(event.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL)) {
                        isSelectingAll = false;
                    }
                } else if(event.type == SDL_TEXTINPUT) {
                    if(!(SDL_GetModState() & KMOD_CTRL)) {
                        hasUnsavedChanges = true;
                        text[currentLine].insert(currentColumn, event.text.text);
                        currentColumn++;
                        cursor.Update();
                        if(cursor.x >= windowWidth) {
                            leftColumn++;
                        }
                    }
                } else if(event.type == SDL_WINDOWEVENT) {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        windowWidth = event.window.data1;
                        windowHeight = event.window.data2;
                        forceRender = true; // Force render the window with resized properties 
                        if(windowWidth < cursor.GetRect().x) {
                            leftColumn = currentColumn - (windowWidth / fontWidth) + (cursor.offset / fontWidth) + 1;
                        }
                        leftColumn = std::min(currentColumn, leftColumn);
                    }
                }
            }

            if(hasUnsavedChanges) {
                SDL_SetWindowTitle(gWindow, (windowTitle + '*').c_str());
            }

            if(forceRender || (currentColumn != previousColumn || currentLine != previousLine || text != previousText)) {
                // Only executes if changes have been detected
                forceRender = false; // Exception, render is executed if this is true
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderClear(gRenderer);

                RenderLineHighlight();
                RenderText();
                RenderCursor(cursor);

                SDL_RenderPresent(gRenderer);
            }

            SDL_Delay(10);
        }

        close();
    } else {
        std::cout << "File not valid, please specify a file to edit\n";
    }
    return 0;

}