CC = clang++
HEART_SOURCE = HeartOfIce.cpp
HEART_OUTPUT = HeartOfIce.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -O3 -std=c++17
INCLUDES = -I/usr/include/SDL2

all: clean heart

heart:
	$(CC) $(HEART_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(HEART_OUTPUT)

clean:
	rm -f *.exe *.o
