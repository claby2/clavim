OSFLAG :=
ifeq ($(OS), Windows_NT)
	executable = test_preferences.exe
else
	executable = ./test_preferences
endif


compile:
	@g++ -std=c++17 -c ../src/preferences.cpp
	@g++ -std=c++17 -c test_preferences.cpp
	@g++ -std=c++17 preferences.o test_preferences.o -o test_preferences
test:
	@$(executable)
all: compile test