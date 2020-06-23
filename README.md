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
2. Run clavim by specifying the path to the file you would like to edit (the following example opens `filename.txt`):
```
$ clavim filename.txt
```

### Keybinds
Key | Action
----|-------
"Up" (the Up arrow key (navigation keypad)) | Move up one line
"Down" (the Down arrow key (navigation keypad)) | Move down one line
"Left" (the Left arrow key (navigation keypad)) | Move left one character
"Right" (the Right arrow key (navigation keypad)) | Move right one character
"Backspace" | Delete character to the left of cursor or remove empty line
"Return" (the Enter key (main keyboard)) | Create a new line after current line
"Ctrl" + "S" | Save changes to file
"Ctrl" + "A" | Select all text
