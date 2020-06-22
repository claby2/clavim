compile:
	@g++ -std=c++11 -I/usr/local/include/ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run: $(file)
	@clavim.exe $(file)
all: $(file)
	@g++ -std=c++11 -I/usr/local/include/ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim && clavim.exe $(file)