LIBS=-lglut -lGLEW -lMagick++ -lassimp -lGL
OPTS=-I/usr/include/ImageMagick -I/usr/local/include/assimp `Magick++-config --cxxflags --cppflags --ldflags --libs`
CC=g++
CCFLAGS=-g -Wall

TARGET=truck
FULL_SOURCES=bone_vertex material mesh texture model player shared vertex world truck_part wehicle
SOURCES=$(FULL_SOURCES) main
HEADERS=$(FULL_SOURCES) libs/INI

HDRS=$(addprefix src/, $(addsuffix .h, $(HEADERS)))
OBJS=$(addprefix obj/, $(addsuffix .o, $(SOURCES)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(OPTS) -o $@ $(OBJS) $(LIBS)

obj/%.o: src/%.cpp $(HDRS)
	$(CC) $(CCFLAGS) $(OPTS) -c $< -o $@ $(LIBS)

clean:
	-rm -rf obj/*.o $(TARGET)
