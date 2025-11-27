# COMP2113-GROUP60
## Race Against the Clock ⏰

### Group Members:
* Hu Xinyue-3036292556

* Jia Qinhuan-3036457223

* Wu Yuerong-3036440579

* Zhang Linghui-3036292544

* Zhou Zhixuan-3036445660

### Game Description:
This is a package delivery game featuring the player, map, packages and customer, with two difficulty levels to choose from.

The positions of the walls on the map are randomly generated at the start of each game, which prohibit player movement.

The positions of the packages and the customer are randomly generated at the start of each game.

The player's goal is to deliver all packages on the map to the customer within the step limit.

Win condition: All packages delivered before running out of steps.

Lose condition: 1. Quit game. 2: There are packages on the map or at hand when 0 steps remain.

### Required Features:
1. Generation of random events: The position of the walls, the packages and the customer are randomly generated.
2. Data structures for storing data: Vectors
3. Dynamic memory management:
4. File input/output: The saveprogress file enables saving and loading of game progress.
5. Multiple files: The following files are included: main.cpp, intro.cpp, intro.h, player.cpp, player.h, saveprogress.cpp, saveprogress.h, end.cpp, end.h.
6. Multiple difficulty levels: Easy mode: 10*10 map, 2 packages,Hard mode: 17*17 map, 5 packages

--------------------------
Instructions:
W-Up movement
S-Down movement
A-Left movement
D-Right movement
Q-Quit game
S-Save progress
Packages are automatically picked up when the player reaches the position and are automatically delivered when the player reaches the customer

--------------------------
Compilation:
1. VS Code: 
Ensure that MinGW-w64 or TDM-GCC is installed.
Install the C/C++ extension in VS Code.
Open the files: main.cpp intro.cpp saveprogress.cpp end.cpp.
Press F5 or click "Run and Debug".
Select the "C++ Debug (g++)" configuration.
2. Command:
g++ main.cpp intro.cpp player.cpp saveprogress.cpp end.cpp -o delivery_game.exe

Execution Instructions:
1. VS Code: Press F5
2. Command: delivery_game.exe








