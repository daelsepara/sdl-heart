CC = clang++
SKULLS_SOURCE = HeartOfIce.cpp
SKULLS_OUTPUT = HeartOfIce.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -O3 -std=c++17
INCLUDES = -I/usr/include/SDL2

all: clean heart

heart:
	$(CC) $(SKULLS_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(SKULLS_OUTPUT)

clean:
	rm -f *.exe *.o
