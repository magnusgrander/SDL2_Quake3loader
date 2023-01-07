CC=g++
CFLAGS=-c -g3 -I /usr/include/freetype2
LDFLAGS= -lGL -lGLU -lGLEW -lSDL2 -ldl -lfreetype 
SOURCES= Main.cpp TextureLoader.cpp Init.cpp Frustum.cpp Camera.cpp Quake3Bsp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
