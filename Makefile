compile:
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run: $(file)
	@clavim.exe $(file)
all: $(file)
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim && clavim.exe $(file)