#ifndef CURSOR_HPP
#define CURSOR_HPP
#include "common.hpp"
#include <SDL2/SDL.h>
#include <string>

class Cursor {
    private:
        int x, y, width, height;
    public:
        int offset;

        Cursor() {
            width = fontWidth;
            height = fontHeight;
        }

        void update(int currentLine, int currentColumn, int leftColumn, int currentTopLine, int maxLineNumber) {
            offset = (fontWidth * (int)std::to_string(maxLineNumber).size()) + fontWidth;
            x = offset + ((currentColumn - leftColumn) * fontWidth);
            y = (currentLine * fontHeight) - (currentTopLine * fontHeight);
        }

        SDL_Rect getRect() {
            SDL_Rect cursorRect = {x, y, width, height};
            return cursorRect;
        }

        /*
        Render cursor
        */
        void render(int currentColumn, int lineLength) { //Render Cursor
            SDL_Rect cursorRect = getRect();
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
            if(currentColumn >= lineLength) {
                SDL_RenderFillRect(gRenderer, &cursorRect);   
            }
            SDL_RenderDrawRect(gRenderer, &cursorRect);
        }
};

#endif