# clavim
A text editor that I do not intend on using.

## Getting Started

### Prerequisites
```
SDL2
SDL2_ttf
```

### Installing

* Install [SDL2](http://libsdl.org/download-2.0.php)
* Install [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

## Usage

1. Clone and compile:
```
$ git clone https://github.com/claby2/clavim.git
$ cd clavim
$ cd src
$ make compile
```
2. Run clavim by specifying the path to the file you would like to edit (a file will be created if it does not already exist):
```
$ clavim filename.txt
```

### Preferences

Optionally, you can set your own preferences to alter the way clavim looks and behaves in `src/preferences.ini`.
Simply change a certain preference in `src/preferences.ini` and save the file.
After this is done, changes should be reflected the next time clavim is run.
If an invalid value is detected (based on a custom validity check), a warning should be written to standard output.
In this case, any preference with an invalid value would be ignored and the default value would be used instead. 

To change the font clavim uses, add the `.ttf` file to the `src` directory and change the `font` preference in `src/preferences.ini` to the name of the font file.

### Keybinds
| Key                                               | Action                                                      |
|---------------------------------------------------|-------------------------------------------------------------|
| "Up" (the Up arrow key (navigation keypad))       | Move up one line                                            |
| "Down" (the Down arrow key (navigation keypad))   | Move down one line                                          |
| "Left" (the Left arrow key (navigation keypad))   | Move left one character                                     |
| "Right" (the Right arrow key (navigation keypad)) | Move right one character                                    |
| "Backspace"                                       | Delete character to the left of cursor or remove empty line |
| "Return" (the Enter key (main keyboard))          | Create a new line after current line                        |
| "Tab" (the Tab key)                               | Create indentation of four spaces                           |
| "Ctrl" + "S"                                      | Save changes to file                                        |
| "Ctrl" + "A"                                      | Select all text                                             |