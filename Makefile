# Email:Edenhassin@gmail.com

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -ICoupGame
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files for CLI (main.cpp is the main)
SRC_MAIN = main.cpp \
           $(wildcard CoupGame/*.cpp) \
           $(wildcard CoupGame/Roles/*.cpp)

# Source files for GUI (GameGUI.cpp is the main)
SRC_GUI = GameGUI.cpp \
          $(wildcard CoupGame/*.cpp) \
          $(wildcard CoupGame/Roles/*.cpp)

# Source files for tests (one combined test executable)
SRC_TEST = PlayerTest.cpp GameTest.cpp \
           $(wildcard CoupGame/*.cpp) \
           $(wildcard CoupGame/Roles/*.cpp)

# Output executable names
TARGET_MAIN = CoupGameApp
TARGET_GUI = CoupGameGUI
TARGET_TEST = AllTestsExec

# Default target
all: Main

# Compile and run MAIN (CLI demo)
Main: $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $^ $(LDFLAGS)
	./$(TARGET_MAIN)

# Compile and run GUI
gui: $(SRC_GUI)
	$(CXX) $(CXXFLAGS) -o $(TARGET_GUI) $^ $(LDFLAGS)
	./$(TARGET_GUI)

# Compile all test files into a single test executable
test: $(TARGET_TEST)
	./$(TARGET_TEST)

$(TARGET_TEST): $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Run valgrind on the unified test binary
valgrind: $(TARGET_TEST)
	valgrind --leak-check=full --track-origins=yes -s ./$(TARGET_TEST)

# Clean build files and executables
clean:
	find . -type f -name '*.o' -delete
	rm -f $(TARGET_MAIN) $(TARGET_GUI) $(TARGET_TEST)

.PHONY: all Main gui test valgrind clean
