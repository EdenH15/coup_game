# 🃏 Coup Game in C++ with SFML GUI

This project is a complete C++ implementation of the strategic bluffing game **Coup**. 
It includes a fully interactive graphical interface using **SFML**, 
modular object-oriented design, and a comprehensive unit test suite to ensure correctness and reliability.


---

## 📁 Project Structure

- `main.cpp` – Demo runner for launching the game logic.
- `Game.cpp` / `Game.h` – Core game mechanics: turns, actions, rules.
- `Player.cpp` / `Player.h` – Player logic: coins, influence, actions.
- `Roles/` – A directory containing all special roles file (`Spy`, `Judge`, `General`, `Baron`, `Governor`, `Marchant`), each inheriting from the `Player` base class.
- `GameGUI.cpp` / `GameGUI.h` – Graphical User Interface built with SFML.
- `GameTest.cpp`, `PlayerTest.cpp` – Unit tests covering core logic and edge cases.
- `Makefile` – Automates compiling, testing, memory checks, and launching the GUI.
- `arial.ttf` – Font used for rendering text in the graphical interface.

---

## 🎮 Game Instructions

Each player starts with a role and must eliminate the others, taking actions, and strategically managing resources.  
Actions include collecting income, blocking opponents, using special abilities, or performing a coup.  
The last player remaining wins the game.  

## 🕹️ Game Rules

Each player draws a secret role card and takes turns performing actions such as gathering coins, bribing, taxing, arresting, or launching coups.  
Special roles (e.g., Governor, Spy, General) can enhance or block actions in unique ways.  
Players aim to eliminate others using strategy and bluffing — the last remaining player wins.  
Actions cost or gain coins and can be blocked; illegal moves raise exceptions.

---
## ✅ Unit Testing
The project includes unit tests that verify:

- Player actions and special abilities
- Turn flow and game progression
- Victory detection and endgame logic
- Handling of illegal actions and boundary cases

---

## 🎨 Graphical Interface

Built with **SFML**, the GUI includes:

- A welcome screen and game start menu
- Player count and name input with validation
- Role reveal stage and role management
- Game board with real-time player status
- Interactive popups for target selection and error feedback

---


## ⚙️ Build & Run Instructions

Use the provided `Makefile` for all compilation and execution tasks:

```bash
make Main        # Run the demonstration (main.cpp)
make test        # Execute all unit tests
make valgrind    # Check for memory leaks with Valgrind
make clean       # Clean up compiled and temporary files
make GUI         # Launch the graphical game (requires SFML)

💡 Make sure SFML is installed to run the GUI version


## 📌 Requirements

- **C++17** or later  
- **SFML 2.5+** installed  

👩‍💻 Author
    Eden Hassin  
    Edenhassin@gmail.com  
    Project completed: May 2025


