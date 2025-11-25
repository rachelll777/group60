#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "player.h"
using namespace std;

bool saveGameProgress(Player* p, const vector<pair<int, int>>& packageLocs, 
                     const pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const string& difficulty) {
    
    ofstream saveFile("game_save.txt");
    if (!saveFile.is_open()) {
        cout << "Error: Cannot create save file!" << endl;
        return false;
    }
    
    // 保存基本信息 - 所有变量名称与main.cpp一致
    saveFile << difficulty << endl;
    saveFile << p->loc.first << " " << p->loc.second << endl;
    saveFile << moveCount << endl;
    saveFile << stepLimits << endl;
    saveFile << numPackages << endl;
    
    // 保存客户位置
    saveFile << customerLoc.first << " " << customerLoc.second << endl;
    
    // 保存玩家库存
    saveFile << p->inventory.size() << endl;
    for (const auto& item : p->inventory) {
        saveFile << item << endl;
    }
    
    // 保存剩余包裹位置
    saveFile << packageLocs.size() << endl;
    for (const auto& loc : packageLocs) {
        saveFile << loc.first << " " << loc.second << endl;
    }
    
    saveFile.close();
    cout << "Game progress saved successfully!" << endl;
    return true;
}

bool loadGameProgress(Player*& p, vector<pair<int, int>>& packageLocs, 
                     pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, string& difficulty) {
    
    ifstream saveFile("game_save.txt");
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // 读取基本信息 - 所有变量名称与main.cpp一致
    saveFile >> difficulty;
    
    // 读取玩家位置
    int playerX, playerY;
    saveFile >> playerX >> playerY;
    p->loc = {playerX, playerY};
    
    // 读取移动信息
    saveFile >> moveCount;
    saveFile >> stepLimits;
    saveFile >> numPackages;
    
    // 读取客户位置
    int custX, custY;
    saveFile >> custX >> custY;
    customerLoc = {custX, custY};
    
    // 读取玩家库存
    p->inventory.clear();
    int inventorySize;
    saveFile >> inventorySize;
    for (int i = 0; i < inventorySize; i++) {
        string item;
        saveFile >> item;
        p->inventory.push_back(item);
    }
    
    // 读取包裹位置
    packageLocs.clear();
    int packageCount;
    saveFile >> packageCount;
    for (int i = 0; i < packageCount; i++) {
        int x, y;
        saveFile >> x >> y;
        packageLocs.push_back({x, y});
    }
    
    saveFile.close();
    cout << "Game loaded successfully!" << endl;
    return true;
}
