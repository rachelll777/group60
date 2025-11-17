#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>

#include <Windows.h>
#pragma execution_character_set("utf-8")

#include <fcntl.h>

using namespace std;

// Game state enumeration
enum GameState {
    MAIN_MENU,
    INSTRUCTIONS,
    DIFFICULTY_SELECT,
    GAME_PROGRESS,
    GAME_PLAYING
};

// Difficulty enumeration
enum Difficulty {
    EASY,    // Small map + 2 packages
    HARD     // Large map + 5 packages
};

// Global variables
int input;
GameState currentState = MAIN_MENU;
int menuSelection = 0; // Main menu selection
int difficultySelection = 0; // Difficulty selection
Difficulty selectedDifficulty = EASY;

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
    "┃  2. Use WASD keys to move on the map                   ┃\n",
    "┃  3. Automatically pick up packages when stepping on    ┃\n",
    "┃     package locations                                  ┃\n",
    "┃  4. Automatically deliver all packages at hand when    ┃\n",
    "┃     stepping on customer location                      ┃\n",
    "┃  5. Press Q to save game at any time                   ┃\n",
    "┃  6. Deliver all packages to customer within step limit ┃\n",
    "┃                                                        ┃\n",
    "┃  Win Condition: Steps remaining >= 0 AND all packages  ┃\n",
    "┃                 delivered to customer                  ┃\n",
    "┃  Lose Condition: Run out of steps with packages        ┃\n",
    "┃                 remaining OR quit game                 ┃\n"
};

vector<string> gameProgress = {
    "┃                     Game Progress                      ┃\n",
    "┃                                                        ┃\n",
    "┃  Current Progress: New Game                            ┃\n",
    "┃  Position: 0,0                                         ┃\n",
    "┃  Steps remaining: n                                    ┃\n", //n is the total number of steps
    "┃  Packages Remaining: 2                                 ┃\n", 
    "┃                                                        ┃\n"
};

// Function declarations
void printMainMenu();
void printInstructions();
void printDifficultySelect();
void printGameProgress();
void startGame();
void handleMainMenuInput();
void handleDifficultyInput();

// Utility functions
string vectorToString(vector<string> arr) {
    string s = "";
    for(string thing : arr) s = s + thing;
    return s; 
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
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
        "┃               │ [2] Difficulty Level    │              ┃\n",
        "┃               ╰─────────────────────────╯              ┃\n"
    };
    vector<string> butt2 = {
        "┃               [2] Difficulty Level                     ┃\n"
    };
    
    vector<string> butt3high = {
        "┃               ╭─────────────────────────╮              ┃\n",
        "┃               │ [3] Game Progress       │              ┃\n",
        "┃               ╰─────────────────────────╯              ┃\n"
    };
    vector<string> butt3 = {
        "┃               [3] Game Progress                        ┃\n"
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
        
    if(menuSelection == 2) 
        for(string line : butt3high) cout << line;
    else 
        for(string line : butt3) cout << line;
    
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

void printGameProgress() {
    string top =    "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =  "┃                                                        ┃\n";
    string bottom = "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
    
    clearScreen();
    cout << top;
    for(string line : gameProgress) cout << line;
    cout << empty;
    cout << "┃              Press Q to return to Main Menu            ┃\n";
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
    cout << "┃               Press Q to save game                     ┃\n";
    cout << "┃               Press ESC to return to menu              ┃\n";
    cout << empty;
    cout << "┃                      Good Luck!                        ┃\n";
    cout << empty;
    cout << bottom;
    
    // Wait for user input
    while(true) {
        input = getch();
        if(input == 27) { // ESC key
            currentState = MAIN_MENU;
            break;
        } else if(input == 113 || input == 81) { // Q key
            cout << "┃                     Progress Saved                     ┃\n";
        }
    }
}

void handleMainMenuInput() {
    input = getch();
    
    if(input == 87 || input == 119) { // W/w
        menuSelection = (menuSelection - 1 + 3) % 3;
    } else if(input == 83 || input == 115) { // S/s
        menuSelection = (menuSelection + 1) % 3;
    } else if(input == 13) { // Enter
        switch(menuSelection) {
            case 0: currentState = INSTRUCTIONS; break;
            case 1: currentState = DIFFICULTY_SELECT; break;
            case 2: currentState = GAME_PROGRESS; break;
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), CP_UTF8);
    cout << fixed << setprecision(2);
    srand(time(0));

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
                    currentState = MAIN_MENU;
                }
                break;
                
            case DIFFICULTY_SELECT:
                printDifficultySelect();
                handleDifficultyInput();
                break;
                
            case GAME_PROGRESS:
                printGameProgress();
                input = getch();
                if(input == 113 || input == 81) { // Q/q
                    currentState = MAIN_MENU;
                }
                break;
                
            case GAME_PLAYING:
                startGame();
                break;
        }
    }
    
    return 0;
}