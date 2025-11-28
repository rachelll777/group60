#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <utility> 

#include "player.h"
using namespace std;

// Saves the current game state into a file for future loading
bool saveGameProgress(Player* p, const vector<pair<int, int> >& packageLocs, 
                     const pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const string& difficulty, const vector<vector<string>>& currentMap) {  

    string filename = "d:\\delivery_game\\game_save.txt";
    
    // delete previously saved file
    remove(filename.c_str());

    ofstream saveFile(filename);
    if (!saveFile.is_open()) {
        cout << "Error: Cannot create save file!" << endl;
        return false;
    }
    
    // 添加调试信息
    cout << "DEBUG: Starting save process..." << endl;
    cout << "DEBUG: Map size: " << currentMap.size() << "x" 
         << (currentMap.empty() ? 0 : currentMap[0].size()) << endl;
    
    // Save basic information
    saveFile << difficulty << endl;
    saveFile << p->loc.first << " " << p->loc.second << endl;
    saveFile << moveCount << endl;
    saveFile << stepLimits << endl;
    saveFile << numPackages << endl;
    
    // Save customer position
    saveFile << customerLoc.first << " " << customerLoc.second << endl;
    
    // Save player inventory
    saveFile << p->inventory.size() << endl;
    for (const auto& item : p->inventory) {
        saveFile << item << endl;
    }
    
    // Save remaining package positions
    saveFile << packageLocs.size() << endl;
    for (const auto& loc : packageLocs) {
        saveFile << loc.first << " " << loc.second << endl;
    }

    // 确保地图数据被保存 - 添加详细调试
    cout << "DEBUG: Saving map data now..." << endl;
    if (currentMap.empty()) {
        cout << "ERROR: Current map is empty!" << endl;
        saveFile.close();
        return false;
    }
    
    cout << "DEBUG: Map dimensions: " << currentMap.size() << "x" << currentMap[0].size() << endl;
    
    // 保存地图尺寸
    saveFile << currentMap.size() << " " << currentMap[0].size() << endl;
    
    // 保存地图内容
    for (int i = 0; i < currentMap.size(); i++) {
        for (int j = 0; j < currentMap[i].size(); j++) {
            // 处理空格字符 - 如果是空格就保存为"_"，避免加载时解析问题
            if (currentMap[i][j] == " ") {
                saveFile << "_ ";
            } else {
                saveFile << currentMap[i][j] << " ";
            }
        }
        saveFile << endl;
        
        // 调试输出前几行
        if (i < 2) {
            cout << "DEBUG: Saved row " << i << endl;
        }
    }
    
    saveFile.close();
    
    // 验证保存结果
    ifstream checkFile(filename);
    if (checkFile.is_open()) {
        string line;
        int lineCount = 0;
        while (getline(checkFile, line)) {
            lineCount++;
        }
        checkFile.close();
        cout << "DEBUG: Save file has " << lineCount << " lines" << endl;
    }
    
    cout << "Game progress saved successfully!" << endl;
    return true;
}

bool loadGameProgress(Player*& p, vector<pair<int, int> >& packageLocs, 
                     pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, string& difficulty, vector<vector<string>>& currentMap) {
    
    string filename = "d:\\AAAdelivery_game\\game_save.txt";
    ifstream saveFile(filename);
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // Load basic information
    saveFile >> difficulty;
    
    // Load player position
    int playerX, playerY;
    saveFile >> playerX;
    saveFile >> playerY;
    p->loc = {playerX, playerY};
    
    // Load move-related information
    saveFile >> moveCount;
    saveFile >> stepLimits;
    saveFile >> numPackages;
    
    // Load customer position
    int custX, custY;
    saveFile >> custX;
    saveFile >> custY;
    customerLoc = {custX, custY};
    
    // Load player inventory
    p->inventory.clear();
    int inventorySize;
    saveFile >> inventorySize;
    saveFile.ignore(); // 跳过换行符
    for (int i = 0; i < inventorySize; i++) {
        string item;
        getline(saveFile, item);
        p->inventory.push_back(item);
    }
    
    // Load remaining package positions
    packageLocs.clear();
    int packageCount;
    saveFile >> packageCount;
    for (int i = 0; i < packageCount; i++) {
        int x, y;
        saveFile >> x >> y;
        packageLocs.push_back({x, y});
    }
    
    // 加载地图数据
    int rows, cols;
    saveFile >> rows >> cols;
    cout << "DEBUG: Loading map of size " << rows << "x" << cols << endl;
    
    currentMap.clear();
    saveFile.ignore(); // 跳过换行符
    
    for (int i = 0; i < rows; i++) {
        string line;
        getline(saveFile, line);
        
        vector<string> row;
        string cell;
        
        // 解析空格分隔的单元格
        for (size_t j = 0; j < line.length(); j++) {
            char c = line[j];
            if (c == ' ') {
                if (!cell.empty()) {
                    // 将"_"转换回空格
                    if (cell == "_") {
                        row.push_back(" ");
                    } else {
                        row.push_back(cell);
                    }
                    cell.clear();
                }
            } else {
                cell += c;
            }
        }
        
        // 处理最后一个单元格
        if (!cell.empty()) {
            if (cell == "_") {
                row.push_back(" ");
            } else {
                row.push_back(cell);
            }
        }
        
        // 验证列数是否正确
        if (row.size() != cols) {
            cout << "WARNING: Row " << i << " has " << row.size() 
                 << " columns, expected " << cols << endl;
        }
        
        currentMap.push_back(row);
    }

    saveFile.close();
    cout << "DEBUG: Loaded map size: " << currentMap.size() << "x" 
         << (currentMap.empty() ? 0 : currentMap[0].size()) << endl;
    cout << "Game loaded successfully!" << endl;
    return true;
}