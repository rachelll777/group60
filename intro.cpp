#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <fstream>

#include <Windows.h>
#include <fcntl.h>
#include "intro.h"

using namespace std;

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>
    
    // getch() for Linux/macOS
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

// Global variable definitions
extern string bGreen, bRed, bYellow, bBlue, bMagenta, bCyan, white;
int input;
GameState currentState = MAIN_MENU;
int menuSelection = 0;
int difficultySelection = 0;
Difficulty selectedDifficulty = EASY;
bool saveFileExists = false;
string saveFileName = "game_save.txt";  // Same name as in saveprogress.cpp

// Game rules text
vector<string> gameRules = {
    bCyan + "┃" + white + "                   " + bYellow + "Game Instructions" + white + "                    " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "1." + white + " Choose difficulty: " + bYellow + "Rookie" + white + "(small map, 2 packages)   " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "     or " + bRed + "Expert" + white + "(large map, 5 packages)                   " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "2." + white + " Use " + bYellow + "WASD" + white + " keys to move on the map (press ENTER)     " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "3." + white + " Automatically pick up packages (" + bGreen + "1" + white + ") when stepping   " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "     on package locations                               " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "4." + white + " Automatically deliver all packages at hand when    " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "     stepping on customer location (" + bCyan + "C" + white + ")                  " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "5." + white + " Press " + bBlue + "V" + white + " to save game at any time                   " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "6." + white + " Press " + bRed + "Q" + white + " to quit game                               " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "7." + white + " Deliver all packages to customer within step limit " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bGreen + "Win Condition:" + white + " Steps remaining >= 0 AND all packages  " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "                 delivered to customer                  " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "  " + bRed + "Lose Condition:" + white + " Run out of steps with packages        " + bCyan + "┃\n" + white,
    bCyan + "┃" + white + "                 remaining OR quit game                 " + bCyan + "┃\n" + white
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
        cout << bRed << "No saved game found!" << white << endl;
        return false;
    }
    
    // Only check basic information here; detailed loading is done in main.cpp
    string difficulty;
    saveFile >> difficulty;
    
    if (difficulty == "easy" || difficulty == "hard") {
        cout << bGreen << "Valid save file detected!" << white << endl;
        saveFile.close();
        return true;
    }
    
    saveFile.close();
    return false;
}

void printMainMenu() {
    string top =    bBlue + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bBlue + "┃" + white + "                                                        " + bBlue + "┃\n" + white;
    string bottom = bBlue + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    
    vector<string> title = {
        bBlue + "┃" + white + "                " + bYellow + "Race Against the Clock!" + white + "                 " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    An excellent courier should deliver packages        " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    quickly and safely! The customer is getting         " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    impatient, find shortcuts to save steps!            " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "                      " + bGreen + "Good Luck!" + white + "                        " + bBlue + "┃\n" + white
    };
    
    vector<string> butt1high = {
        bBlue + "┃" + white + "               " + bGreen + "╭─────────────────────────╮" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "│" + bYellow + " [1] Game Instructions   " + bGreen + "│" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "╰─────────────────────────╯" + white + "              " + bBlue + "┃\n" + white
    };
    vector<string> butt1 = {
        bBlue + "┃" + white + "               " + bYellow + "[1] Game Instructions" + white + "                    " + bBlue + "┃\n" + white
    };
    
    vector<string> butt2high = {
        bBlue + "┃" + white + "               " + bGreen + "╭─────────────────────────╮" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "│" + bYellow + " [2]  New Game           " + bGreen + "│" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "╰─────────────────────────╯" + white + "              " + bBlue + "┃\n" + white
    };
    vector<string> butt2 = {
        bBlue + "┃" + white + "               " + bYellow + "[2]  New Game" + white + "                            " + bBlue + "┃\n" + white
    };

    vector<string> butt3high = {
        bBlue + "┃" + white + "               " + bGreen + "╭─────────────────────────╮" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "│" + bCyan + " [3]  Continue Game      " + bGreen + "│" + white + "              " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "               " + bGreen + "╰─────────────────────────╯" + white + "              " + bBlue + "┃\n" + white
    };
    vector<string> butt3 = {
        bBlue + "┃" + white + "               " + bCyan + "[3]  Continue Game" + white + "                       " + bBlue + "┃\n" + white
    };
    
    string controls = bBlue + "┃" + white + "    " + bGreen + "W/S:" + white + " Navigate    " + bYellow + "Enter:" + white + " Confirm    " + bRed + "Q:" + white + " Back          " + bBlue + "┃\n" + white;

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
    string top =    bCyan + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white;
    string bottom = bCyan + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    
    clearScreen();
    cout << top;
    for(string line : gameRules) cout << line;
    cout << empty;
    cout << bCyan + "┃" + white + "              Press " + bRed + "Q" + white + " to return to Main Menu            " + bCyan + "┃\n" + white;
    cout << empty;
    cout << bottom;
}

void printDifficultySelect() {
    string top =    bYellow + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bYellow + "┃" + white + "                                                        " + bYellow + "┃\n" + white;
    string bottom = bYellow + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    
    vector<string> title = {
        bYellow + "┃" + white + "                  " + bCyan + "Select Difficulty" + white + "                     " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "                                                        " + bYellow + "┃\n" + white
    };
    
    vector<string> easyHigh = {
        bYellow + "┃" + white + "              " + bGreen + "╭─────────────────────────────╮" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bGreen + "│" + bYellow + " [1] Rookie Mode- Small Map  " + bGreen + "│" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bGreen + "│" + bYellow + "     2 Packages              " + bGreen + "│" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bGreen + "╰─────────────────────────────╯" + white + "           " + bYellow + "┃\n" + white
    };
    vector<string> easyNormal = {
        bYellow + "┃" + white + "              " + bGreen + "[1] Rookie Mode - Small Map (2 Packages)" + white + "  " + bYellow + "┃\n" + white
    };
    
    vector<string> hardHigh = {
        bYellow + "┃" + white + "              " + bRed + "╭─────────────────────────────╮" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bRed + "│" + bYellow + " [2] Expert Mode - Large Map " + bRed + "│" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bRed + "│" + bYellow + "     5 Packages              " + bRed + "│" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bRed + "╰─────────────────────────────╯" + white + "           " + bYellow + "┃\n" + white
    };
    vector<string> hardNormal = {
        bYellow + "┃" + white + "              " + bRed + "[2] Expert Mode - Large Map (5 Packages)" + white + "  " + bYellow + "┃\n" + white
    };
    
    vector<string> startHigh = {
        bYellow + "┃" + white + "              " + bBlue + "╭─────────────────────────────╮" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bBlue + "│" + bGreen + " [3] Start Game              " + bBlue + "│" + white + "           " + bYellow + "┃\n" + white,
        bYellow + "┃" + white + "              " + bBlue + "╰─────────────────────────────╯" + white + "           " + bYellow + "┃\n" + white
    };
    vector<string> startNormal = {
        bYellow + "┃" + white + "              " + bBlue + "[3] Start Game" + white + "                            " + bYellow + "┃\n" + white
    };
    
    string controls = bYellow + "┃" + white + "    " + bGreen + "W/S:" + white + " Navigate    " + bYellow + "Enter:" + white + " Confirm    " + bRed + "Q:" + white + " Back          " + bYellow + "┃\n" + white;

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
    string top =    bGreen + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bGreen + "┃" + white + "                                                        " + bGreen + "┃\n" + white;
    string bottom = bGreen + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    
    cout << top << empty;
    cout << bGreen + "┃" + white + "            " + bYellow + "Ready to Race Against the Clock?" + white + "            " + bGreen + "┃\n" + white;
    cout << empty;
    
    if(selectedDifficulty == EASY) {
        cout << bGreen + "┃" + white + "               " + bCyan + "You are a rookie courier" + white + "                 " + bGreen + "┃\n" + white;
        cout << bGreen + "┃" + white + "               " + bCyan + "New to the small town" + white + "                    " + bGreen + "┃\n" + white;
        cout << bGreen + "┃" + white + "               " + bCyan + "With 2 packages to deliver" + white + "               " + bGreen + "┃\n" + white;
    } else {
        cout << bGreen + "┃" + white + "               " + bRed + "You are an expert courier" + white + "                " + bGreen + "┃\n" + white;
        cout << bGreen + "┃" + white + "               " + bRed + "Who is familiar with the city" + white + "            " + bGreen + "┃\n" + white;
        cout << bGreen + "┃" + white + "               " + bRed + "With 5 packages to deliver" + white + "               " + bGreen + "┃\n" + white;
    }
    
    cout << empty;
    cout << bGreen + "┃" + white + "               Use " + bYellow + "WASD" + white + " to move                         " + bGreen + "┃\n" + white;
    cout << bGreen + "┃" + white + "               Press " + bBlue + "V" + white + " to save game                     " + bGreen + "┃\n" + white;
    cout << bGreen + "┃" + white + "               Press " + bRed + "ESC" + white + " to return to menu              " + bGreen + "┃\n" + white;
    cout << bGreen + "┃" + white + "               Press " + bGreen + "ENTER" + white + " to start game                " + bGreen + "┃\n" + white;
    cout << empty;
    cout << bGreen + "┃" + white + "                      " + bYellow + "Good Luck!" + white + "                        " + bGreen + "┃\n" + white;
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
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), CP_UTF8);
    cout << fixed << setprecision(2);
    srand(time(0));

    // Check if a save file exists - using the same name as in saveprogress.cpp
    saveFileExists = checkSaveFile();
    if(saveFileExists) {
        cout << bGreen << "Save file found! 'Continue Game' option enabled." << white << endl;
        // Verify if the save file is valid
        if (loadGameFromSave()) {
            cout << bGreen << "Valid save file detected. You can continue your game." << white << endl;
        } else {
            cout << bRed << "Save file is corrupted. Starting new game." << white << endl;
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
                cout << bCyan << "Loading game from save file..." << white << endl;
                sleep(2);
                return 2;   // Return 2 to load saved game
                break;
        }
    }
    
    return 0;
}