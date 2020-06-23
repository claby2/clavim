#include "common.hpp"
#include "cursor.hpp"
#include "text_area.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <fstream>

TextArea::TextArea() {
    // Input existing content of file into text vector
    file.open(saveFilePath.c_str(), std::ios::in);
    std::string line;
    int lineNumber = 0;
    while(std::getline(file, line)) {
        if(lineNumber > text.size() - 1) text.push_back("");
        text[lineNumber] = line;
        lineNumber++;
    }
    file.close();

    cursor.update(currentLine, currentColumn, leftColumn, currentTopLine, text.size());
}


void TextArea::save() { // SaveTextToFile
    if(hasUnsavedChanges) {
        file.open(saveFilePath.c_str(), std::ios::out);
        for(int i = 0; i < text.size(); i++) {
            file << text[i] << "\n";
        }
        file.close();
        hasUnsavedChanges = false;
        SDL_SetWindowTitle(gWindow, windowTitle.c_str());
    }
}

void TextArea::renderLineHighlight() {
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

void TextArea::renderText() {
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

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        if(width > windowWidth) {
            SDL_Rect endLineHighlight = {
                windowWidth - fontWidth,
                ((i * fontHeight) - (currentTopLine * fontHeight)),
                fontWidth,
                fontHeight
            };
            SDL_RenderDrawRect(gRenderer, &endLineHighlight);
        }

        if(leftColumn > 0) {
            SDL_Rect beginLineHighlight = {
                cursor.offset,
                ((i * fontHeight) - (currentTopLine * fontHeight)),
                fontWidth,
                fontHeight
            };
            SDL_RenderDrawRect(gRenderer, &beginLineHighlight);
        }
    }
}

bool TextArea::shouldIncreaseCurrentTopLine() {
    return ((currentLine * fontHeight) - (currentTopLine * fontHeight) >= windowHeight);
}

bool TextArea::shouldDecreaseCurrentTopLine() {
    return ((currentLine * fontHeight) - (currentTopLine * fontHeight) < 0);
}

void TextArea::setColumn(int newValue) {
    newValue = std::max(newValue, 0);
    newValue = std::min(newValue, (int)text[currentLine].length());
    currentColumn = newValue;
    leftColumn = std::min(currentColumn, leftColumn);
    cursor.update(currentLine, currentColumn, leftColumn, currentTopLine, text.size());
    if(cursor.getRect().x >= windowWidth) {
        leftColumn++;
    }
}

void TextArea::setLine(int newValue) {
    if(newValue >= 0 && newValue + 1 <= text.size()) {
        setColumn(std::min(static_cast<int>(text[newValue].length()), currentColumn));
        currentLine = newValue;
        if(shouldDecreaseCurrentTopLine()) currentTopLine--;
        else if(shouldIncreaseCurrentTopLine()) currentTopLine++;                
    }
    cursor.update(currentLine, currentColumn, leftColumn, currentTopLine, text.size());
}

bool TextArea::removeChar() {
    if(text[currentLine].length() > 0) { // If there is at least one character in the line
        if(currentColumn) { // If there is a character before the cursor (which is to be deleted)
            text[currentLine].erase(text[currentLine].begin() + currentColumn - 1);
            return true;
        } else {
            if(currentLine) {
                // Concatenate the contents of current line to the end of the previous line and delete current line
                text[currentLine - 1] += text[currentLine];
                setColumn(text[currentLine - 1].length());
                text.erase(text.begin() + currentLine); // Erase current line
                setLine(currentLine - 1);
            }
        }
    } else if(text.size() > 1){ // If there are at least 2 lines, it means the current line can be deleted
        text.erase(text.begin() + currentLine); // Erase current line
        setLine(currentLine - 1);
    }
    return false;
}

void TextArea::up() {
    setLine(currentLine - 1);
}

void TextArea::down() {
    setLine(currentLine + 1);
}

void TextArea::left() {
    setColumn(currentColumn - 1);
}

void TextArea::right() {
    setColumn(currentColumn + 1);
}

void TextArea::backspace() {
    hasUnsavedChanges = true;
    if(isSelectingAll) { // If the user is selecting entire text
        currentColumn = 0;
        currentLine = 0;
        text.clear();
        text.push_back("");
        leftColumn = 0;
    } else {
        if(removeChar()) left();
    }
}

void TextArea::newLine() {
    std::string newLineString = "";
    hasUnsavedChanges = true;
    if(currentColumn < text[currentLine].length()) {
        newLineString = text[currentLine].substr(currentColumn);
        text[currentLine] = text[currentLine].substr(0, currentColumn);
    }
    text.insert(text.begin() + currentLine + 1, newLineString);
    down();
    setColumn(0);
}

void TextArea::selectAll() {
    isSelectingAll = true;
    forceRender = true;
}

void TextArea::removeSelection() {
    if(isSelectingAll) forceRender = true;
    isSelectingAll = false;
}

void TextArea::inputText(std::string eventText) {
    hasUnsavedChanges = true;
    text[currentLine].insert(currentColumn, eventText);
    right();
}

void TextArea::resizeWindow(int newWidth, int newHeight) {
    windowWidth = newWidth;
    windowHeight = newHeight;
    forceRender = true; // Force render the window with resized properties 
    if(windowWidth < cursor.getRect().x) {
        leftColumn = currentColumn - (windowWidth / fontWidth) + (cursor.offset / fontWidth) + 1;
    }
    leftColumn = std::min(currentColumn, leftColumn);
}

void TextArea::setPrevious() {
    previousLine = currentLine;
    previousColumn = currentColumn;
    previousText = text;
}

void TextArea::render() {
    cursor.update(currentLine, currentColumn, leftColumn, currentTopLine, text.size());
    if(hasUnsavedChanges) {
        SDL_SetWindowTitle(gWindow, (windowTitle + '*').c_str());
    }

    if(forceRender || (currentColumn != previousColumn || currentLine != previousLine || text != previousText)) {
        // Only executes if changes have been detected
        forceRender = false; // Exception, render is executed if this is true
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        renderLineHighlight();
        renderText();
        cursor.render(currentColumn, text[currentLine].length());

        SDL_RenderPresent(gRenderer);
    }
}