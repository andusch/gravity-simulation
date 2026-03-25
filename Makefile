# Variables
CXX = clang++
CXXFLAGS = -std=c++17 -Wall
INCLUDES = -I./dependencies/include
LIBS = -L./dependencies/lib -lglfw.3
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# dependencies/lib folder for runtime
RPATH = -Wl,-rpath,$(shell pwd)/dependencies/lib

# Target names
TARGET = main
SRC = main.cpp glad.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(LIBS) $(FRAMEWORKS) $(RPATH) -o $(TARGET)

clean:
	rm -f $(TARGET)