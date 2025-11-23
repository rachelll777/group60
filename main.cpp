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

pair<int, int> playerLoc = {};
pair<int, int> customerLoc = {};
int numPackages;
vector<pair<int, int>> packageLocs = {};
map<vector<int>, int> allSolutions = {};

// 在地图上随机放置元素
void placeItemsRandomly() {
    pair<int, int> loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    vector<pair<int, int>> usedLocs = {};
    // player location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    playerLoc = loc;
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
void displayMapInfo() {
    cout << "\033[2J\033[1;1H";
    cout << "Press W, A, S, D to move, or press Q to quit." << endl;
    for(int i = 0; i < currentMap.size(); i++) {
        for(int j = 0; j < currentMap[i].size(); j++) {
            pair<int, int> loc = {i, j};
            bool isPackage = false;
            int packageNum = -1;
            for(int i = 0; i < packageLocs.size(); i++) {
                if(packageLocs[i] == loc) {
                    isPackage = true;
                    packageNum = i;
                }
            }
            if(loc == playerLoc) cout << "P ";
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

bool checkValidMovement(pair<int ,int> dir) {
    pair<int, int> newLoc = {playerLoc.first+dir.first, playerLoc.second+dir.second};
    if(dir.first == 0 && dir.second == 0) {
        /*
         * Do interaction
         */
        return true;
    } else if(currentMap[newLoc.first][newLoc.second] == "#") return false;
    else return true;
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
int shortestDelivery() {
    vector<bool> visited(numPackages+1, false);
    vector<vector<string>> path = {};
    vector<int> defaultPath = {};
    for(int i = 0; i < numPackages; i++) defaultPath.push_back(i);
    permute(defaultPath, 0, numPackages-1);
    int distance = 99999;
    for(vector<int> path : allPaths) {
        int currDist = pathFinding(playerLoc, packageLocs[path[0]]);
        for(int i = 0; i < path.size()-1; i++) currDist = currDist+pathFinding(packageLocs[path[i]], packageLocs[path[i+1]]);
        currDist = currDist+pathFinding(packageLocs[path[numPackages-1]], customerLoc);
        if(currDist < distance) distance = currDist;
        allSolutions[path] = currDist;
    }
    return distance;
}

int main() {
    showMainMenu();
    
    srand(time(0));

    if(startGame) {
        cout << "Start game!" << endl;
        
        currentMap = (difficulty=="easy")?easyBaseMapTemplate:hardBaseMapTemplate;
        numPackages = (difficulty=="easy")?2:5;
        placeItemsRandomly();
        
        int moveCount = 0;
        int minSteps = shortestDelivery();
        stepLimits = (difficulty=="easy")?minSteps*2:minSteps;

        choice = "";

        while(choice != "q" && choice != "Q") {
            displayMapInfo();
            cout << "Move count " << moveCount << endl;
            cout << "Move limit " << stepLimits << endl;
            cin >> choice;

            if(choice == "Q" || choice == "q") break;
            if(checkValidKey(choice)) {
                pair<int, int> dir = {};
                if(choice == "W" || choice == "w") dir = {-1, 0};
                else if(choice == "A" || choice == "a") dir = {0, -1};
                else if(choice == "S" || choice == "s") dir = {1, 0};
                else if(choice == "D" || choice == "d") dir = {0, 1};
                else dir = {0, 0};

                if(checkValidMovement(dir)) {
                    playerLoc = {playerLoc.first+dir.first, playerLoc.second+dir.second};
                    moveCount = moveCount+1;
                } else cout << "Not a valid move." << endl;

            } else cout << "Not a valid key." << endl;
            
            if(moveCount > stepLimits) {
                cout << "Oh no you lost!" << endl;
                break;
            }
            // cout << move << endl;
            
            
        }

    }

    return 0;
}
