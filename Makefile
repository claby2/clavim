run:
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim && clavim.exe
compile:
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim