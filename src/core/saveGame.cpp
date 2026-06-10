#include "../../include/core/saveGame.h"
#include <cstring>

SaveGame::SaveGame() 
    : saveID(""), timestamp(0), username(""), playerID(-1), gameTimer(0.0f),
      totalPointsEarnedThisGame(0), playerX(0), playerY(0), 
      currentPlayerTotalPoints(0), bonusMultiplierCount(0), powerUpCount(0),
      powerUpActive(false), powerUpTimeRemaining(0.0f), enemyCount(0),
      minSpeed(0), maxSpeed(0) {
    
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 40; j++)
            grid[i][j] = 0;
    
    for (int i = 0; i < 10; i++) {
        enemies[i] = {0, 0, 0, 0};
    }
}

SaveGame::~SaveGame() {
}

void SaveGame::setGrid(int g[25][40]) {
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 40; j++)
            grid[i][j] = g[i][j];
}

void SaveGame::getGrid(int g[25][40]) const {
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 40; j++)
            g[i][j] = grid[i][j];
}

void SaveGame::setEnemy(int index, int x, int y, int dx, int dy) {
    if (index >= 0 && index < 10) {
        enemies[index].x = x;
        enemies[index].y = y;
        enemies[index].dx = dx;
        enemies[index].dy = dy;
    }
}