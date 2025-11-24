#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

#include <unistd.h>

#include <map>
#include <queue>
#include <stack>

#include "player.h"

// 跨平台键盘输入处理
#ifdef _WIN32
    #include <conio.h>  // Windows 使用 conio.h
#else
    // macOS/Linux 使用 termios 实现 21getch()
    #include <termios.h>
    #include <stdio.h>
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

bool startGame = false;
string difficulty = "";

bool winGame = false;

string choice;

int moveCount;
int stepLimits;

// Easy模式 10x10
vector<vector<string>> easyBaseMapTemplate = {
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", "#", " ", " ", " ", " ", " ", "#"},
        {"#", "#", " ", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", "#", " ", "#"},
        {"#", " ", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", " ", "#"},
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#"}
    };

// Hard模式 17x17
vector<vector<string>> hardBaseMapTemplate = {
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", " ", "#", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", "#", " ", " ", " ", "#", " ", "#", " ", " ", " ", "#", " ", "#"},
        {"#", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", "#"},
        {"#", "#", "#", "#", "#", " ", "#", "#", "#", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", "#", "#", "#", "#", " ", "#", "#", "#", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#"}
    };


vector<vector<string>> currentMap;

pair<int, int> customerLoc = {};
int numPackages;
vector<pair<int, int>> packageLocs = {};
bool deliveringPackage = false;
int numDeliveredPackages = -1;

map<vector<int>, int> allSolutions = {};



// 在地图上随机放置元素
void placeItemsRandomly(Player *&p) {
    pair<int, int> loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    vector<pair<int, int>> usedLocs = {};
    // player location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    p->loc = loc;
    usedLocs.push_back(loc);
    
    
    // customer location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    customerLoc = loc;
    usedLocs.push_back(loc);

    for(int i = 0; i < numPackages; i++) {
        while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
        packageLocs.push_back(loc);
        usedLocs.push_back(loc);
    }
}

// Not used! 生成指定模式的地图
// vector<vector<string>> generateMap(string mode) {
//     vector<vector<string>> currentMap;
//     if (mode == "easy") {
//         currentMap = easyBaseMapTemplate;
//         placeItemsRandomly(currentMap, 2, 1); // 2个物品，1个顾客
//     } else if (mode == "hard") {
//         currentMap = hardBaseMapTemplate;
//         placeItemsRandomly(currentMap, 5, 1); // 5个物品，1个顾客
//     }
//     return currentMap;
// }

// 显示地图和物品信息
void displayMapInfo(Player *&p) {
    cout << "\033[2J\033[1;1H";
    cout << "Press W, A, S, D to move, or press Q to quit." << endl;
    for(int i = 0; i < currentMap.size(); i++) {
        for(int j = 0; j < currentMap[i].size(); j++) {
            pair<int, int> loc = {i, j};
            bool isPackage = false;
            int packageNum = 0;
            for(int i = 0; i < packageLocs.size(); i++) {
                if(packageLocs[i] == loc) {
                    isPackage = true;
                    break;
                }
            }
            if(loc == p->loc) cout << "P ";
            else if(loc == customerLoc) cout << "C ";
            else if(isPackage) cout << (packageNum+1) << " ";
            else cout << currentMap[i][j] << " ";
        }
        cout << endl;
    }
}

// 主菜单
void showMainMenu() {
    cout << "=== MAZE EXPLORATION GAME ===" << endl;
    cout << "1. Easy Mode (10x10 currentMap)" << endl;
    cout << "   - 2 Items to collect" << endl;
    cout << "   - 1 Customer to find" << endl;
    cout << "2. Hard Mode (17x17 currentMap)" << endl;
    cout << "   - 5 Items to collect" << endl;
    cout << "   - 1 Customer to find" << endl;
    cout << "3. Exit Game" << endl;
    cout << "=============================" << endl;
    cout << "Please select an option (1-3): ";
    cin >> choice;

    if(choice == "1") {
        difficulty = "easy";
        startGame = true;
    } else if(choice == "2") {
        difficulty = "hard";
        startGame = true;
    } else if(choice == "3") {
        cout << "Thank you for playing! Goodbye!" << endl;
        startGame = false;
    } else {
        showMainMenu();
        cout << "Invalid choice! Please select 1, 2, or 3." << endl;
    }
}

bool checkValidKey(string choice) {
    return choice == "W" || choice =="w" || choice =="A" || choice =="a" || choice == "S" || choice =="s" || choice =="D" || choice =="d" || choice ==" ";
}

bool checkValidMovement(Player *&p, pair<int ,int> dir) {
    return currentMap[p->loc.first+dir.first][p->loc.second+dir.second] != "#";
}

vector<vector<int>> allPaths = {};

void permute(vector<int> path, int start, int end) {
    if(start == end) {
        allPaths.push_back(path);
        return;
    } else {
        for(int i = start; i <= end; i++) {
            swap(path[start], path[i]);
            permute(path, start+1, end);
            swap(path[start], path[i]);
        }
    }
}


// Compute shortest path between 2 locations
// Input: pair<int, int> loc1, pair<int, int> loc2 
// Implementation: bfs
int pathFinding(pair<int, int> loc1, pair<int, int> loc2) {
    vector<vector<bool>> visited(currentMap.size(), vector<bool>(currentMap[0].size(), false));
    visited[loc1.first][loc1.second] = true;
    queue<vector<int>> q;
    q.push({loc1.first, loc1.second, 0});
    
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while(!q.empty()) {
        int x = q.front()[0], y = q.front()[1], distance = q.front()[2];
        q.pop();
        
        if(x == loc2.first && y == loc2.second) return distance;
        for(vector<int> dir : directions) {
            int nextX = x+dir[0], nextY = y+dir[1];
            if(currentMap[nextX][nextY] != "#" && !visited[nextX][nextY]) {
                visited[nextX][nextY] = true;
                q.push({nextX, nextY, distance+1});
            }
        }
    }
    return -1;
}

// 0, 1, C
// 0, 1, 2, 3, 4, C
int shortestDelivery(Player *&p) {
    vector<bool> visited(numPackages+1, false);
    vector<vector<string>> path = {};
    vector<int> defaultPath = {};
    for(int i = 0; i < numPackages; i++) defaultPath.push_back(i);
    permute(defaultPath, 0, numPackages-1);
    int distance = 99999;
    for(vector<int> path : allPaths) {
        int currDist = pathFinding(p->loc, packageLocs[path[0]]);
        for(int i = 0; i < path.size()-1; i++) currDist = currDist+pathFinding(packageLocs[path[i]], packageLocs[path[i+1]]);
        currDist = currDist+pathFinding(packageLocs[path[numPackages-1]], customerLoc);
        if(currDist < distance) distance = currDist;
        allSolutions[path] = currDist;
    }
    return distance;
}


// Player functions, all of them, write it here.
// display current inventory
void displayInv(Player *&p) {
    cout << "📦 Inventory: ";
    if (p->inventory.empty()){

        cout << "empty";}
    else{
        for (const auto& pkg : p->inventory) cout << pkg << " ";}
    cout << endl;
}

// add package to inventory
void addPackage(Player *&p, string package) {
    p->inventory.push_back(package);
    cout << "Player picked up Package number " << package << endl;
    
    packageLocs.erase(packageLocs.begin()+stoi(package)-1);
    for(pair<int, int> i : packageLocs) {
        cout << i.first << " " << i.second << endl;
    }
    displayInv(p);
}

 // count packages
int getPackageCount(Player *&p) {
    return p->inventory.size();
}

// check if inventory is empty
bool hasPackages(Player *&p){
    return !p->inventory.empty();
}

// deliver to customer
void deliverToCustomer(Player *&p, vector<vector<string>>& map, pair<int, int> loc) {
    // 1. check if player location is same as customer
    // Ambiguous. no need to check if player inventory is empty or not
    // 2. Remove package from player's inventory
    // check customer's location
    if (p->loc == customerLoc) {
        if (p->inventory.size() != 0) {
            deliveringPackage = true;
            numDeliveredPackages = p->inventory.size();
            p->inventory.clear();
        }
    }
}

// pick up packages when reach packages
void pickupPackageFromMap(Player *&p) {
    // 1: check which package player is standing
    bool isOnPackage = false;
    int packNum = -1;
    for(int i = 0; i < packageLocs.size(); i++) {
        pair<int, int> packLoc = packageLocs[i];
        if(packLoc == p->loc) {
            isOnPackage = true;
            packNum = i+1;
            p->inventory.push_back(to_string(packNum));
            packageLocs.erase(packageLocs.begin()+i);
            break;
        }
    }
    // 2: remove that package from the list of packages -> it wont be printed out again
    
}


int main() {
    showMainMenu();
    
    srand(time(0));

    if(startGame) {
        cout << "Start game!" << endl;
        
        currentMap = (difficulty=="easy")?easyBaseMapTemplate:hardBaseMapTemplate;
        numPackages = (difficulty=="easy")?2:7;
        
        Player *p = new Player();
        p->inventory = {};
        p->moveCount = 0;

        placeItemsRandomly(p);
        
        int moveCount = 0;
        int minSteps = shortestDelivery(p);
        stepLimits = (difficulty=="easy")?minSteps*2:minSteps+2;

        
        p->moveLimit = stepLimits;

        choice = "";

        while(choice != "q" && choice != "Q") {
            displayMapInfo(p);
            cout << "Move count " << moveCount << endl;
            cout << "Move limit " << stepLimits << endl;
            cout << "Player inventory: { ";
            for(int i = 0; i < p->inventory.size();i++) cout << "| Package " << (i+1) << " | ";
            cout << "}" << endl;
            if(deliveringPackage) {
                cout << "Successfully delivered " << numDeliveredPackages << "package(s) to customer!" << endl;
                numDeliveredPackages = -1;
                deliveringPackage = false;
            }
            cout << endl << endl;

            cin >> choice;

            if(choice == "Q" || choice == "q") break;
            if(checkValidKey(choice)) {
                pair<int, int> dir = {};
                if(choice == "W" || choice == "w") dir = {-1, 0};
                else if(choice == "A" || choice == "a") dir = {0, -1};
                else if(choice == "S" || choice == "s") dir = {1, 0};
                else if(choice == "D" || choice == "d") dir = {0, 1};

                if(checkValidMovement(p, dir)) {
                    //standard moving
                    p->loc = {p->loc.first+dir.first, p->loc.second+dir.second};
                    moveCount = moveCount+1;
                    //picking up item
                    pickupPackageFromMap(p);
                    
                    //passing item
                    deliverToCustomer(p,currentMap, p->loc);

                    if(packageLocs.empty() && p->inventory.empty()) {
                        winGame = true;
                        break;
                    }
                    
                } else cout << "Not a valid move." << endl;

            } else cout << "Not a valid key." << endl;
            
            if(moveCount > stepLimits) {
                cout << "Oh no you lost!" << endl;
                break;
            }
            // cout << move << endl;
            
            
        }
        if(winGame) {
            cout << "Yay you win!" << endl;
        }
    }

    return 0;
}
