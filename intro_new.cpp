#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>        // 替代 math.h
#include <cstdlib>      // 替代 stdlib.h
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include "intro.h"

// ==== Cross-platform keyboard input & Windows console setup ==== 
#ifdef _WIN32          // 在 Windows 上编译时走这里
    #include <conio.h>
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
#else                  // 在 Linux / macOS（cs server）上走这里
    #include <termios.h>
    #include <unistd.h>

    // 自己实现一个 getch()，模仿 Windows 的无回车按键读取
    int getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

using namespace std;

// Global variable definitions
int input;
GameState currentState = MAIN_MENU;
int menuSelection = 0;
int difficultySelection = 0;
Difficulty selectedDifficulty = EASY;
bool saveFileExists = false;
string saveFileName = "game_save.txt";  // Same name as in saveprogress.cpp

string bGreen = "\033[1;32m";
string bRed = "\033[1;31m";
string bYellow = "\033[1;33m";
string bBlue = "\033[1;34m";
string white = "\033[0m";

// Game rules text
vector<string> gameRules = {
    "┃                   Game Instructions                    ┃\n",
    "┃                                                        ┃\n",
    "┃  1. Choose difficulty: Rookie(small map, 2 packages)   ┃\n",
    "┃     or Expert(large map, 5 packages)                   ┃\n",
    "┃  2. Use WASD keys to move on the map (press ENTER)     ┃\n",
    "┃  3. Automatically pick up packages (1) when stepping   ┃\n",
    "┃     on package locations                               ┃\n",
    "┃  4. Automatically deliver all packages at hand when    ┃\n",
    "┃     stepping on customer location (C)                  ┃\n",
    "┃  5. Press V to save game at any time                   ┃\n",
    "┃  6. Press Q to quit game                               ┃\n",
    "┃  7. Deliver all packages to customer within step limit ┃\n",
    "┃                                                        ┃\n",
    "┃  Win Condition: Steps remaining >= 0 AND all packages  ┃\n",
    "┃                 delivered to customer                  ┃\n",
    "┃  Lose Condition: Run out of steps with packages        ┃\n",
    "┃                 remaining OR quit game                 ┃\n"
};

// Utility functions
string vectorToString(vector<string> arr) {
    string s = "";
    for(string thing : arr) s = s + thing;
    return s; 
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

bool checkSaveFile() {
    ifstream file(saveFileName);
    return file.good();
}

// New helper: check if a valid save file exists (basic validation)
bool loadGameFromSave() {
    ifstream saveFile(saveFileName);
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // Only check basic information here; detailed loading is done in main.cpp
    string difficulty;
    saveFile >> difficulty;
    
    if (difficulty == "easy" || difficulty == "hard") {
        cout << "Valid save file detected!" << endl;
        saveFile.close();
        return true;
    }
    
    saveFile.close();
    return false;
}

void printMainMenu() {
    string top =    "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =  "┃                                                        ┃\n";
    string bottom = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    
    vector<string> title = {
        "┃                Race Against the Clock!                 ┃\n",
        "┃    An excellent courier should deliver packages        ┃\n",
        "┃    quickly and safely! The customer is getting         ┃\n",
        "┃    impatient, find shortcuts to save steps!            ┃\n",
        "┃                      Good Luck!                        ┃\n"
    };
    
    vector<string> butt1high = {
        "┃               ╭─────────────────────────╮              ┃\n",
        "┃               │ [1] Game Instructions   │              ┃\n",
        "┃               ╰─────────────────────────╯              ┃\n"
    };
    vector<string> butt1 = {
        "┃               [1] Game Instructions                    ┃\n"
    };
    
    vector<string> butt2high = {
    "┃               ╭─────────────────────────╮              ┃\n",
    "┃               │ [2]  New Game           │              ┃\n",  // 改为Continue Game
    "┃               ╰─────────────────────────╯              ┃\n"
    };
    vector<string> butt2 = {
    "┃               [2]  New Game                            ┃\n"  // 改为Continue Game
    };

    vector<string> butt3high = {
    "┃               ╭─────────────────────────╮              ┃\n",
    "┃               │ [3]  Continue Game      │              ┃\n",  // 改为New Game
    "┃               ╰─────────────────────────╯              ┃\n"
    };
    vector<string> butt3 = {
    "┃               [3]  Continue Game                       ┃\n"  // 改为New Game
    };
    
    string controls = "┃    W/S: Navigate    Enter: Confirm    Q: Back          ┃\n";

    clearScreen();
    cout << top << empty;
    for(string line : title) cout << line;
    cout << empty;
    
    // Display menu options
    if(menuSelection == 0) 
        for(string line : butt1high) cout << line;
    else 
        for(string line : butt1) cout << line;
        
    if(menuSelection == 1) 
        for(string line : butt2high) cout << line;
    else 
        for(string line : butt2) cout << line;

    // only show the 'Continue Game' option when previous progress was saved
    if(saveFileExists) {
        if(menuSelection == 2) 
            for(string line : butt3high) cout << line;
        else 
            for(string line : butt3) cout << line;
    }
    
    cout << empty << empty;
    cout << controls;
    cout << empty;
    cout << bottom;
}

void printInstructions() {
    string top =    "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =  "┃                                                        ┃\n";
    string bottom = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    
    clearScreen();
    cout << top;
    for(string line : gameRules) cout << line;
    cout << empty;
    cout << "┃              Press Q to return to Main Menu            ┃\n";
    cout << empty;
    cout << bottom;
}

void printDifficultySelect() {
    string top =    "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =  "┃                                                        ┃\n";
    string bottom = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    
    vector<string> title = {
        "┃                  Select Difficulty                     ┃\n",
        "┃                                                        ┃\n"
    };
    
    vector<string> easyHigh = {
        "┃              ╭─────────────────────────────╮           ┃\n",
        "┃              │ [1] Rookie Mode- Small Map  │           ┃\n",
        "┃              │     2 Packages              │           ┃\n",
        "┃              ╰─────────────────────────────╯           ┃\n"
    };
    vector<string> easyNormal = {
        "┃              [1] Rookie Mode - Small Map (2 Packages)  ┃\n"
    };
    
    vector<string> hardHigh = {
        "┃              ╭─────────────────────────────╮           ┃\n",
        "┃              │ [2] Expert Mode - Large Map │           ┃\n",
        "┃              │     5 Packages              │           ┃\n",
        "┃              ╰─────────────────────────────╯           ┃\n"
    };
    vector<string> hardNormal = {
        "┃              [2] Expert Mode - Large Map (5 Packages)  ┃\n"
    };
    
    vector<string> startHigh = {
        "┃              ╭─────────────────────────────╮           ┃\n",
        "┃              │ [3] Start Game              │           ┃\n",
        "┃              ╰─────────────────────────────╯           ┃\n"
    };
    vector<string> startNormal = {
        "┃              [3] Start Game                            ┃\n"
    };
    
    string controls = "┃    W/S: Navigate    Enter: Confirm    Q: Back          ┃\n";

    clearScreen();
    cout << top;
    for(string line : title) cout << line;
    
    // Display difficulty options
    if(difficultySelection == 0) 
        for(string line : easyHigh) cout << line;
    else 
        for(string line : easyNormal) cout << line;
        
    if(difficultySelection == 1) 
        for(string line : hardHigh) cout << line;
    else 
        for(string line : hardNormal) cout << line;
        
    if(difficultySelection == 2) 
        for(string line : startHigh) cout << line;
    else 
        for(string line : startNormal) cout << line;
    
    cout << empty << empty;
    cout << controls;
    cout << empty;
    cout << bottom;
}

void startGame() {
    clearScreen();
    string top =    "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =  "┃                                                        ┃\n";
    string bottom = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    
    cout << top << empty;
    cout << "┃            Ready to Race Against the Clock?            ┃\n";
    cout << empty;
    
    if(selectedDifficulty == EASY) {
        cout << "┃               You are a rookie courier                 ┃\n";
        cout << "┃               New to the small town                    ┃\n";
        cout << "┃               With 2 packages to deliver               ┃\n";
    } else {
        cout << "┃               You are an expert courier                ┃\n";
        cout << "┃               Who is familiar with the city            ┃\n";
        cout << "┃               With 5 packages to deliver               ┃\n";
    }
    
    cout << empty;
    cout << "┃               Use WASD to move                         ┃\n";
    cout << "┃               Press V to save game                     ┃\n";
    cout << "┃               Press ESC to return to menu              ┃\n";
    cout << "┃               Press ENTER to start game                ┃\n";
    cout << empty;
    cout << "┃                      Good Luck!                        ┃\n";
    cout << empty;
    cout << bottom;
    
    while(true) {
        input = getch();
        if(input == 27) { // ESC key - return to main menu
            currentState = MAIN_MENU;
            break;
        } else if(input == 13) { // ENTER key 
            break;  // Exit loop and let runIntroLoop return 1
        }
    }
}

void handleMainMenuInput() {
    input = getch();
    
    // Dynamically compute the number of menu items
    int menuItemCount = saveFileExists ? 3 : 2; // With save: 3 options, without save: 2 options
    
    if(input == 87 || input == 119) { // W/w
        menuSelection = (menuSelection - 1 + menuItemCount) % menuItemCount;
    } else if(input == 83 || input == 115) { // S/s
        menuSelection = (menuSelection + 1) % menuItemCount;
    } else if(input == 13) { // Enter
        switch(menuSelection) {
            case 0: 
                currentState = INSTRUCTIONS; 
                break;
            case 1: 
                if(saveFileExists) {
                      // Option 1: New Game (select difficulty and start new game)
                    currentState = DIFFICULTY_SELECT;
                } else {
                   // Without save file: Option 1 is also New Game
                    currentState = DIFFICULTY_SELECT;
                }
                break;
            case 2:
               // Option 2: Continue Game (load saved progress)
                currentState = LOAD_GAME;
                break;
        }
    } else if(input == 113 || input == 81) { // Q/q
        // Already in main menu, no action needed
    }
}

void handleDifficultyInput() {
    input = getch();
    
    if(input == 87 || input == 119) { // W/w
        difficultySelection = (difficultySelection - 1 + 3) % 3;
    } else if(input == 83 || input == 115) { // S/s
        difficultySelection = (difficultySelection + 1) % 3;
    } else if(input == 13) { // Enter
        if(difficultySelection == 0) {
            selectedDifficulty = EASY;
        } else if(difficultySelection == 1) {
            selectedDifficulty = HARD;
        } else if(difficultySelection == 2) {
            currentState = GAME_PLAYING;
        }
    } else if(input == 113 || input == 81) { // Q/q
        currentState = MAIN_MENU;
    }
}

// Main intro/menu loop function - replaces the original main()
int runIntroLoop() {
#ifdef _WIN32   // Only on Windows
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);
#endif

    cout << fixed << setprecision(2);
    srand(time(0));

    // Check if a save file exists - using the same name as in saveprogress.cpp
    saveFileExists = checkSaveFile();
    if(saveFileExists) {
        cout << "Save file found! 'Continue Game' option enabled." << endl;
        // Verify if the save file is valid
        if (loadGameFromSave()) {
            cout << "Valid save file detected. You can continue your game." << endl;
        } else {
            cout << "Save file is corrupted. Starting new game." << endl;
            saveFileExists = false;
        }
        sleep(2); 
    }

    while(true) {
        switch(currentState) {
            case MAIN_MENU:
                printMainMenu();
                handleMainMenuInput();
                break;
                
            case INSTRUCTIONS:
                printInstructions();
                input = getch();
                if(input == 113 || input == 81) { // Q/q
                // Return to main menu
                    currentState = MAIN_MENU;
                }
                break;
                
            case DIFFICULTY_SELECT:
                printDifficultySelect();
                handleDifficultyInput();
                break;
                
            case GAME_PLAYING:
                startGame();
                if (currentState == MAIN_MENU) {
                    break;
                } else {
                    return 1;// Return 1 to start a new game
                }
                break;

            case LOAD_GAME:
                cout << "Loading game from save file..." << endl;
                sleep(2);
                return 2;   // Return 2 to load saved game
                break;
        }
    }
    
    return 0;
}
