# קומפיילר ודגלים
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -ICoupGame
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# קבצי מקור של CLI (main.cpp הוא ה-main)
SRC_MAIN = main.cpp \
           $(wildcard CoupGame/*.cpp) \
           $(wildcard CoupGame/Roles/*.cpp)

# קבצי מקור של GUI (GameGUI.cpp הוא ה-main)
SRC_GUI = GameGUI.cpp \
          $(wildcard CoupGame/*.cpp) \
          $(wildcard CoupGame/Roles/*.cpp)

# קבצי מקור לבדיקות
SRC_TEST = tests/Test.cpp \
           $(wildcard CoupGame/*.cpp) \
           $(wildcard CoupGame/Roles/*.cpp)

# שמות קבצי הפלט
TARGET_MAIN = CoupGameApp
TARGET_GUI = CoupGameGUI
TARGET_TEST = CoupGameTests

# ברירת מחדל
all: Main

# קומפילציה והרצה של MAIN (הדמו CLI)
Main: $(SRC_MAIN)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $^ $(LDFLAGS)
	./$(TARGET_MAIN)

# קומפילציה והרצה של GUI
gui: $(SRC_GUI)
	$(CXX) $(CXXFLAGS) -o $(TARGET_GUI) $^ $(LDFLAGS)
	./$(TARGET_GUI)

# קומפילציה והרצת בדיקות
test: $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $^ $(LDFLAGS)
	./$(TARGET_TEST)

# בדיקת זיכרון עם valgrind על MAIN
valgrind: Main
	valgrind --leak-check=full --track-origins=yes ./$(TARGET_MAIN)

# ניקוי
clean:
	find . -type f -name '*.o' -delete
	rm -f $(TARGET_MAIN) $(TARGET_GUI) $(TARGET_TEST)
