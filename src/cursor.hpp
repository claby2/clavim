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

        Cursor();

        /*
        Update cursor x, y, and offset based on line and column positioning
        */
        void update(int currentLine, int currentColumn, int leftColumn, int currentTopLine, int maxLineNumber);

        /*
        Get SDL rect with properties of cursor
        */
        SDL_Rect getRect();

        /*
        Render cursor
        */
        void render(int currentColumn, int lineLength);
};

#endif