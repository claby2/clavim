OSFLAG :=
ifeq ($(OS), Windows_NT)
	executable = clavim.exe
else
	executable = ./clavim
endif

compile:
	@g++ -std=c++11 -I/usr/local/include/ clavim.cpp -lSDL2main -lSDL2 -lSDL2_ttf -o clavim
run: $(file)
	@$(executable) $(file)
all: $(file)
	@g++ -std=c++11 -I/usr/local/include/ clavim.cpp -lSDL2main -lSDL2 -lSDL2_ttf -o clavim && $(executable) $(file)