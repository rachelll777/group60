#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <cstdlib>
#include <ctime>
#include <unistd.h>      // for sleep on Linux

#include "player_new.h"
#include "intro_new.h"
#include "end_new.h"
#include "saveprogress_new.h"

using namespace std;


// Cross-platform keyboard input handling
#ifdef _WIN32
    #include <conio.h>  // Windows uses conio.h
#else
    // macOS/Linux uses termios to implement getch()
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

bool winGame = false;
int moveCount =0;
int stepLimits =0;
extern Difficulty selectedDifficulty;

// Easy mode 10x10 map
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

// Hard mode 17x17 map
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

// Randomly place items on the map
void placeItemsRandomly(Player *&p) {
    pair<int, int> loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    vector<pair<int, int>> usedLocs = {};
    // player location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
        loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    p->loc = loc;
    usedLocs.push_back(loc);
    
    // customer location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
        loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    customerLoc = loc;
    usedLocs.push_back(loc);

    for(int i = 0; i < numPackages; i++) {
        while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
            loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
        packageLocs.push_back(loc);
        usedLocs.push_back(loc);
    }
}

// Display map and item info
void displayMapInfo(Player *&p) {
    cout << "\033[2J\033[1;1H";
    cout << "Controls: W/A/S/D to move, V to save, Q to quit" << endl;
    for(int i = 0; i < currentMap.size(); i++) {
        for(int j = 0; j < currentMap[i].size(); j++) {
            pair<int, int> loc = {i, j};
            bool isPackage = false;
            for(size_t k = 0; k < packageLocs.size(); k++) {
                if(packageLocs[k] == loc) {
                    isPackage = true;
                    break;
                }
            }
            if(loc == p->loc) cout << "P ";
            else if(loc == customerLoc) cout << "C ";
            else if(isPackage) cout << "1 ";
            else cout << currentMap[i][j] << " ";
        }
        cout << endl;
    }
}

bool checkValidKey(string choice) {
    return choice == "W" || choice =="w" || choice =="A" || choice =="a" || 
           choice == "S" || choice =="s" || choice =="D" || choice =="d" || 
           choice == "Q" || choice == "q" || choice == "V" || choice == "v" ||
           choice == " ";
}

bool checkValidMovement(Player *&p, pair<int ,int> dir) {
    int newX = p->loc.first + dir.first;
    int newY = p->loc.second + dir.second;
    return (newX >= 0 && newX < currentMap.size() && 
            newY >= 0 && newY < currentMap[0].size() && 
            currentMap[newX][newY] != "#");
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
            if(nextX >= 0 && nextX < currentMap.size() && 
               nextY >= 0 && nextY < currentMap[0].size() &&
               currentMap[nextX][nextY] != "#" && !visited[nextX][nextY]) {
                visited[nextX][nextY] = true;
                q.push({nextX, nextY, distance+1});
            }
        }
    }
    return -1;
}

// Calculate the shortest delivery path 
int shortestDelivery(Player *&p) {
    vector<bool> visited(numPackages+1, false);
    vector<int> defaultPath = {};
    for(int i = 0; i < numPackages; i++) defaultPath.push_back(i);
    
    allPaths.clear();
    permute(defaultPath, 0, numPackages-1);
    
    int distance = 99999;
    for(vector<int> path : allPaths) {
        int currDist = pathFinding(p->loc, packageLocs[path[0]]);
        for(int i = 0; i < path.size()-1; i++) 
            currDist += pathFinding(packageLocs[path[i]], packageLocs[path[i+1]]);
        currDist += pathFinding(packageLocs[path[numPackages-1]], customerLoc);
        if(currDist < distance) distance = currDist;
    }
    return distance;
}

// Player functions
void displayInv(Player *&p) {
    cout << "Packages Inventory: ";
    if (p->inventory.empty()){
        cout << "empty";
    } else {
        for (const auto& pkg : p->inventory) cout << pkg << " ";
    }
    cout << endl;
}

// deliver to customer
void deliverToCustomer(Player *&p, vector<vector<string>>& map, pair<int, int> loc) {
    if (p->loc == customerLoc) {
        if (p->inventory.size() != 0) {
            deliveringPackage = true;
            numDeliveredPackages = p->inventory.size();
            p->inventory.clear();
        }
    }
}

// pick up packages when reaching packages
void pickupPackageFromMap(Player *&p) {
    for(size_t i = 0; i < packageLocs.size(); i++) {
        pair<int, int> packLoc = packageLocs[i];
        if(packLoc == p->loc) {
            p->inventory.push_back(to_string(i+1));
            packageLocs.erase(packageLocs.begin()+i);
            cout << "Picked up package " << (i+1) << "!" << endl;
            break;
        }
    }
}

int main() {
     // Load intro menu
    int gameStartType = runIntroLoop();
    
    srand(time(0));

    if(gameStartType == 1 || gameStartType == 2) { // New game or Load game
        cout << "Starting game..." << endl;
        sleep(1);
        
        // Use selected difficulty from intro
        string difficulty = (selectedDifficulty == EASY) ? "easy" : "hard";
        
        currentMap = (difficulty=="easy") ? easyBaseMapTemplate : hardBaseMapTemplate;
        numPackages = (difficulty=="easy") ? 2 : 5;
        
        Player *p = new Player();
        p->inventory = {};
        p->moveCount = 0;

         // If loading saved game
        if(gameStartType == 2) {
            cout << "Loading saved game..." << endl;
            if(!loadGameProgress(p, packageLocs, customerLoc, moveCount, stepLimits, numPackages, difficulty)) {
                cout << "Failed to load game. Starting new game." << endl;
                placeItemsRandomly(p);
                int minSteps = shortestDelivery(p);
                stepLimits = (difficulty=="easy") ? minSteps*2 : minSteps+2;
            } else {
                cout << "Game loaded successfully!" << endl;
            }
        } else {
            // Else start new game
            cout << "Starting new game..." << endl;
            placeItemsRandomly(p);
            int minSteps = shortestDelivery(p);
            stepLimits = (difficulty=="easy") ? minSteps*2 : minSteps+2;
        }

        p->moveLimit = stepLimits;
        string choice = "";

        while(choice != "q" && choice != "Q") {
            displayMapInfo(p);
            cout << "==========================================" << endl;
            cout << "Move count: " << moveCount << " / " << stepLimits << endl;
            cout << "Packages remaining: " << packageLocs.size() << " / " << numPackages << endl;
            cout << "Player inventory: { ";
            for(size_t i = 0; i < p->inventory.size(); i++) 
                cout << "| Package " << p->inventory[i] << " | ";
            cout << "}" << endl;
            
            if(deliveringPackage) {
                cout << "🎉 Successfully delivered " << numDeliveredPackages << " package(s) to customer!" << endl;
                numDeliveredPackages = -1;
                deliveringPackage = false;
            }
            
            cout << "==========================================" << endl;
            cout << "Enter move (W/A/S/D), V to save, Q to quit: ";

            cin >> choice;

            if(choice == "Q" || choice == "q") break;
            
            // saving function
            if(choice == "V" || choice == "v") {
                if(saveGameProgress(p, packageLocs, customerLoc, moveCount, stepLimits, numPackages, difficulty)) {
                    cout << "💾 Game saved successfully!" << endl;
                } else {
                    cout << "❌ Failed to save game!" << endl;
                }
                sleep(1);
                continue;
            }
            
            if(checkValidKey(choice)) {
                pair<int, int> dir = {};
                if(choice == "W" || choice == "w") dir = {-1, 0};
                else if(choice == "A" || choice == "a") dir = {0, -1};
                else if(choice == "S" || choice == "s") dir = {1, 0};
                else if(choice == "D" || choice == "d") dir = {0, 1};

                if(checkValidMovement(p, dir)) {
                    p->loc = {p->loc.first+dir.first, p->loc.second+dir.second};
                    moveCount = moveCount+1;
                    pickupPackageFromMap(p);
                    deliverToCustomer(p, currentMap, p->loc);

                    // Check win condition
                    if(packageLocs.empty() && p->inventory.empty()) {
                        winGame = true;
                        break;
                    }
                    
                } else {
                    cout << "❌ Not a valid move. You hit a wall!" << endl;
                    sleep(1);
                }

            } else {
                cout << "❌ Not a valid key. Use W/A/S/D to move." << endl;
                sleep(1);
            }
            
            // Check loss condition (out of moves)
            if(moveCount > stepLimits) {
                cout << "⏰ You ran out of moves!" << endl;
                sleep(1);
                break;
            }
        }
        
        // Show end screen
        int packagesDelivered = numPackages - packageLocs.size();
        showEndScreen(winGame, moveCount, stepLimits, packagesDelivered, numPackages);

    // Delete save file when the game ends
        cout << "Clearing save data..." << endl;
        remove("game_save.txt");
        cout << "Save file deleted successfully." << endl;

        delete p;
        
        //  Exit message before quitting the game
        cout << "Exiting the game..." << endl;
        sleep(2);
    }

    cout << "Thank you for playing!" << endl;
    return 0;
}
