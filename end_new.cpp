#include <iostream>
#include <string>
#include <cstdio>
#include "end1.h"

void showEndScreen(bool winGame, int moveCount, int stepLimits,
                   int packagesDelivered, int totalPackages) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "================================================\n";
    std::cout << "                 GAME OVER\n";
    std::cout << "================================================\n";

    if (winGame) {
        std::cout << "🎉 CONGRATULATIONS! YOU WIN! 🎉\n";
        std::cout << "You delivered all " << packagesDelivered << " packages!\n";
        std::cout << "Moves used: " << moveCount << "/" << stepLimits << "\n";
        std::cout << "Excellent delivery service!\n";
    } else {
        std::cout << "💀 GAME OVER - YOU LOST 💀\n";
        if (moveCount > stepLimits) {
            std::cout << "You ran out of moves!\n";
            std::cout << "Moves used: " << moveCount << "/" << stepLimits << "\n";
        } else {
            std::cout << "You quit the game.\n";
        }
        std::cout << "Better luck next time!\n";
    }

    std::cout << "================================================\n";
    std::cout << "Press ENTER to exit...";
    std::getchar();
}
