OSFLAG :=
ifeq ($(OS), Windows_NT)
	executable = clavim.exe
else
	executable = ./clavim
endif

compile:
	@g++ -std=c++11 -c preferences.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf common.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 cursor.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf text_area.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf clavim.cpp
	@g++ -std=c++11 -I/usr/local/include/ preferences.o common.o cursor.o text_area.o clavim.o -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run: $(file)
	@$(executable) $(file)
all: compile run