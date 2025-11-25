#ifndef INTRO_H
#define INTRO_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

// 全局变量声明
extern int input;
extern GameState currentState;
extern int menuSelection;
extern int difficultySelection;
extern Difficulty selectedDifficulty;
extern bool saveFileExists;
extern string saveFileName;

// 颜色代码
extern string bGreen;
extern string bRed;
extern string bYellow;
extern string bBlue;
extern string white;

// 游戏规则文本
extern vector<string> gameRules;

// 工具函数声明
string vectorToString(vector<string> arr);
void clearScreen();
bool checkSaveFile();

// 界面显示函数
void printMainMenu();
void printInstructions();
void printDifficultySelect();
void startGame();

// 输入处理函数
void handleMainMenuInput();
void handleDifficultyInput();

// 存档相关函数
bool loadGameFromSave();

// 主循环函数 - 供其他文件调用
int runIntroLoop();

#endif