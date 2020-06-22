compile:
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run:
	@clavim.exe $(file)
all:
	@g++ clavim.cpp -w -lSDL2main -lSDL2 -lSDL2_ttf -o clavim && clavim.exe $(file)

ifndef file
$(error File not valid, please specify a file to edit)
endif