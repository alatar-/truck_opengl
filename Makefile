LIBS=-lglut -lGLEW -lMagick++ -lassimp -lGL
TEST_LIBS=$(LIBS) -lcppunit
OPTS=-I/usr/include/ImageMagick -I/usr/local/include/assimp `Magick++-config --cxxflags --cppflags --ldflags --libs`
CC=g++
CCFLAGS=-g -Wall

TARGET=truck
FULL_SOURCES=bone_vertex material mesh texture model camera shared vertex world truck_part vehicle rectangle obstacle
SOURCES=$(FULL_SOURCES) main
HEADERS=$(FULL_SOURCES) libs/INI

TEST_FULL_SOURCES=rectangle
TEST_SOURCES=$(TEST_FULL_SOURCES) main
TEST_HEADERS=$(TEST_FULL_SOURCES)
TEST_TARGET=test

HDRS=$(addprefix src/, $(addsuffix .h, $(HEADERS)))
OBJS=$(addprefix obj/, $(addsuffix .o, $(SOURCES)))

TEST_OBJS=$(addprefix obj/tests/, $(addsuffix .o, $(TEST_SOURCES))) $(addprefix obj/, $(addsuffix .o, $(FULL_SOURCES)))
TEST_HDRS=$(addprefix src/tests/, $(addsuffix .h, $(TEST_HEADERS)))



all: $(TARGET)

test: $(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(OPTS) -o $@ $(OBJS) $(LIBS)

obj/%.o: src/%.cpp $(HDRS)
	$(CC) $(CCFLAGS) $(OPTS) -c $< -o $@ $(LIBS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CCFLAGS) $(OPTS) -o $@ $(TEST_OBJS) $(TEST_LIBS)

obj/tests/%.o: src/tests/%.cpp $(HDRS) $(TEST_HDRS)
	$(CC) $(CCFLAGS) $(OPTS) -c $< -o $@ $(TEST_LIBS)


clean:
	-rm -rf obj/*.o obj/tests/$(TARGET) $(TEST_TARGET)
