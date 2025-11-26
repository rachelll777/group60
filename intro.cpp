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
#include <fstream>

#include <Windows.h>
#include <fcntl.h>
#include "intro.h"

using namespace std;

// 全局变量定义
int input;
GameState currentState = MAIN_MENU;
int menuSelection = 0;
int difficultySelection = 0;
Difficulty selectedDifficulty = EASY;
bool saveFileExists = false;
string saveFileName = "game_save.txt";  // 改为与saveprogress.cpp一致

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

// 新增：从存档加载游戏的函数
bool loadGameFromSave() {
    ifstream saveFile(saveFileName);
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // 这里只检查存档文件的基本信息，具体加载逻辑在main.cpp中实现
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
        if(input == 27) { // ESC key - 返回主菜单
            currentState = MAIN_MENU;
            break;
        } else if(input == 13) { // ENTER key 
            break;  // 跳出循环，让runIntroLoop返回1
        }
    }
}

void handleMainMenuInput() {
    input = getch();
    
    // 动态计算菜单选项总数
    int menuItemCount = saveFileExists ? 3 : 2; // 有存档：3选项，无存档：2选项
    
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
                    // ✅ 修正：选项1是 New Game（选择难度开始新游戏）
                    currentState = DIFFICULTY_SELECT;
                } else {
                    // 无存档时：选项1是选择难度（新游戏）
                    currentState = DIFFICULTY_SELECT;
                }
                break;
            case 2:
                // ✅ 修正：选项2是 Continue Game（加载存档）
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

// 新增的主循环函数 - 替换原来的main函数
int runIntroLoop() {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), CP_UTF8);
    cout << fixed << setprecision(2);
    srand(time(0));

    // 检查存档文件是否存在 - 使用与saveprogress.cpp相同的文件名
    saveFileExists = checkSaveFile();
    if(saveFileExists) {
        cout << "Save file found! 'Continue Game' option enabled." << endl;
        // 验证存档文件是否有效
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
                    return 1;
                }
                break;

            case LOAD_GAME:
                cout << "Loading game from save file..." << endl;
                sleep(2);
                return 2;  // 返回2表示加载游戏
                break;
        }
    }
    
    return 0;
}