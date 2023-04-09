TARGET_EXEC := main
SOURCES := $(shell find . -name '*.cpp')
HEADERS := $(shell find . -name '*.hpp')
INCLUDE_DIR := -I/opt/homebrew/include
LIB_DIR = -L/opt/homebrew/lib

$(TARGET_EXEC): $(SOURCES) $(HEADERS)
	g++ -std=c++11 -o $(TARGET_EXEC) $(SOURCES) $(INCLUDE_DIR) $(LIB_DIR) -lSDL2 -lSDL2_image

clean:
	rm $(TARGET_EXEC)
