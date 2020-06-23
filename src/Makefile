OSFLAG :=
ifeq ($(OS), Windows_NT)
	executable = clavim.exe
else
	executable = ./clavim
endif

compile:
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf common.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 cursor.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf text_area.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf clavim.cpp
	@g++ -std=c++11 -I/usr/local/include/ common.o cursor.o text_area.o clavim.o -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run: $(file)
	@$(executable) $(file)
all: $(file)
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf common.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 cursor.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf text_area.cpp
	@g++ -std=c++11 -c -lSDL2main -lSDL2 -lSDL2_ttf clavim.cpp
	@g++ -std=c++11 -I/usr/local/include/ common.o cursor.o text_area.o clavim.o -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
	@$(executable) $(file)