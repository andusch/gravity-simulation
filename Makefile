# Variables
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -MMD -MP
INCLUDES = -I./dependencies/include

UNAME_S := $(shell uname)
ifeq ($(UNAME_S),Darwin)
LIBS = -L./dependencies/lib -lglfw.3
else
LIBS = -L./dependencies/lib -lglfw.3 -lstdc++fs
endif

FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# dependencies/lib folder for runtime
RPATH = -Wl,-rpath,$(shell pwd)/dependencies/lib

# Target names
TARGET = main
SRC = main.cpp src/Camera.cpp glad.c

OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
DEP = $(OBJ:.o=.d)

all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBS) $(FRAMEWORKS) $(RPATH) -o $(TARGET)

# Rule for compiling C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule for compiling C files (glad.c)
%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# The "Magic" line: include the generated dependency files
-include $(DEP)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)

.PHONY: all clean