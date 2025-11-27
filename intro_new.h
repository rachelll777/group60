#ifndef INTRO_H
#define INTRO_H

#include <iostream>
#include <string>
#include <vector>

// Game state enumeration
enum GameState {
    MAIN_MENU,
    INSTRUCTIONS,
    DIFFICULTY_SELECT,
    GAME_PLAYING,
    LOAD_GAME
};

// Difficulty enumeration
enum Difficulty {
    EASY,    // Small map + 2 packages
    HARD     // Large map + 5 packages
};

// Global variable declarations
extern int input;
extern GameState currentState;
extern int menuSelection;
extern int difficultySelection;
extern Difficulty selectedDifficulty;
extern bool saveFileExists;
extern std::string saveFileName;

// Color code strings
extern std::string bGreen;
extern std::string bRed;
extern std::string bYellow;
extern std::string bBlue;
extern std::string white;

// Game rules text
extern std::vector<std::string> gameRules;

// Utility function declarations
std::string vectorToString(std::vector<std::string> arr);
void clearScreen();
bool checkSaveFile();

// Screen / UI display functions
void printMainMenu();
void printInstructions();
void printDifficultySelect();
void startGame();

// Input handling functions
void handleMainMenuInput();
void handleDifficultyInput();

// Save file related function
bool loadGameFromSave();

// Main loop function - callable from other files
int runIntroLoop();

#endif
