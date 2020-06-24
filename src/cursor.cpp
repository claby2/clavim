#include "common.hpp"
#include "cursor.hpp"
#include <SDL2/SDL.h>
#include <string>

#include <iostream>

Cursor::Cursor() {
    width = fontWidth;
    height = fontHeight;
}

void Cursor::update(int currentLine, int currentColumn, int leftColumn, int currentTopLine, int maxLineNumber) {
    offset = (fontWidth * (int)std::to_string(maxLineNumber).size()) + fontWidth;
    x = offset + ((currentColumn - leftColumn) * fontWidth);
    y = (currentLine * fontHeight) - (currentTopLine * fontHeight);
}

SDL_Rect Cursor::getRect() {
    SDL_Rect cursorRect = {x, y, width, height};
    return cursorRect;
}

void Cursor::render(int currentColumn, int lineLength) {
    SDL_Rect cursorRect = getRect();
    SDL_SetRenderDrawColor(gRenderer, CURSOR_COLOR.r, CURSOR_COLOR.g, CURSOR_COLOR.b, 0x00);
    if(currentColumn >= lineLength) {
        SDL_RenderFillRect(gRenderer, &cursorRect);   
    }
    SDL_RenderDrawRect(gRenderer, &cursorRect);
}