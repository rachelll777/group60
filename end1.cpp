#include <iostream>
#include <string>
using namespace std;

void showEndScreen(bool winGame, int moveCount, int stepLimits, 
                  int packagesDelivered, int totalPackages) {
    cout << "\033[2J\033[1;1H"; 
    cout << "================================================" << endl;
    cout << "           GAME OVER" << endl;
    cout << "================================================" << endl;
     // Check whether the player has won the game
    if (winGame) {
        cout << "🎉 CONGRATULATIONS! YOU WIN! 🎉" << endl;
        cout << "You delivered all " << packagesDelivered << " packages!" << endl;
        cout << "Moves used: " << moveCount << "/" << stepLimits << endl;
        cout << "Excellent delivery service!" << endl;
    }  else { // Check whether the player has won the game
        cout << "💀 GAME OVER - YOU LOST 💀" << endl;
        if (moveCount > stepLimits) {
            cout << "You ran out of moves!" << endl;
            cout << "Moves used: " << moveCount << "/" << stepLimits << endl;
        } else {
            cout << "You quit the game." << endl;
        }
        cout << "Better luck next time!" << endl;
    }
    
    cout << "================================================" << endl;
    cout << "Press any key to exit the game...";
    getchar();
    getchar(); // Wait for user input before exiting
}
