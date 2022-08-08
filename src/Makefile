CC = clang++
HEART_SOURCE = HeartOfIce.cpp
HEART_OUTPUT = HeartOfIce.exe
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LINKER_FLAGS += -stdlib=libc++
	INCLUDES += -I/usr/local/include/SDL2
else
	LINKER_FLAGS += -lstdc++fs
endif

all: clean heart

heart:
	$(CC) $(HEART_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(HEART_OUTPUT)

clean:
	rm -f *.exe *.o
