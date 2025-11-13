TARGET = GoldenMind

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ilib/include

SRC_DIRS = main/src lib/src
BUILD_DIR = build

SOURCES := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

distclean: clean

.PHONY: all clean distclean
