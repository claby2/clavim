#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP
#include "common.hpp"
#include "cursor.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <fstream>

class TextArea {
    public:
        Cursor cursor;
        int currentLine = 0;                          // The line the user is currently on
        int currentColumn = 0;                        // The column the user is current on
        int currentTopLine = 0;                       // The line which is rendered at the top of the text
        int leftColumn = 0;                           // Column number of the column present at the start of the screen (without offset)
        std::vector<std::string> text = {""};         // Stores text of file, each string elements represents one line
        int previousLine;                             // Equal to current line at the start of the main loop to detect changes
        int previousColumn;                           // Equal to current column at the start of the main loop to detect changes
        std::vector<std::string> previousText = {""}; // Equal to text vector at the start of the main loop to detect changes

        bool hasUnsavedChanges = false;               // Represents if the user has made changes to the file and has not saved
        bool isSelectingAll = false;                  // Represents if the user is selecting all due to shortcut
        bool forceRender = true;                      // Force render on start up despite no changes being made

        TextArea();

        /*
        Write the content of text vector into the file
        */
        void save();

        /*
        Given a line the user is working one and the line which appears at the top, highlights the line an alternate color to the background
        */
        void renderLineHighlight();

        /*
        Renders text
        */
        void renderText();

        /*
        Returns if the currentLine value will mean the line would be out of the screen
        */
        bool shouldIncreaseCurrentTopLine();

        /*
        Returns if the currentLine value will mean the line would be out of the screen
        */
        bool shouldDecreaseCurrentTopLine();

        /*
        Set current column to a new value while taking bounds into account, also updates cursor
        */
        void setColumn(int newValue);

        /*
        Set current line to a new value while taking bounds,top line and column into account, also updates cursor
        */
        void setLine(int newValue);

        /*
        Remove a char before current column, returns true if char has been deleted, returns false if line has been deleted
        */
        bool removeChar();

        void up();

        void down();

        void left();

        void right();

        void backspace();

        void newLine();

        void selectAll();

        void removeSelection();

        void inputText(std::string eventText);

        void resizeWindow(int newWidth, int newHeight);

        void setPrevious();

        void render();     
};
#endif