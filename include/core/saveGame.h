#pragma once
#include <string>
#include <ctime>
using namespace std;

struct SavedEnemy {
    int x, y, dx, dy;
};

class SaveGame {
private:
    string saveID;
    time_t timestamp;
    string username;
    int playerID;
    
    
    float gameTimer;
    int totalPointsEarnedThisGame;
    int playerX, playerY;
    int grid[25][40];
    
    
    int currentPlayerTotalPoints;
    int bonusMultiplierCount;
    int powerUpCount;
    bool powerUpActive;
    float powerUpTimeRemaining;
    
    
    int enemyCount;
    SavedEnemy enemies[10];
    
    
    int minSpeed, maxSpeed;

public:
    SaveGame();
    ~SaveGame();
    
    
    void setSaveID(const string& id) { saveID = id; }
    void setTimestamp(time_t t) { timestamp = t; }
    void setUsername(const string& u) { username = u; }
    void setPlayerID(int id) { playerID = id; }
    void setGameTimer(float t) { gameTimer = t; }
    void setTotalPointsEarnedThisGame(int p) { totalPointsEarnedThisGame = p; }
    void setPlayerPosition(int x, int y) { playerX = x; playerY = y; }
    void setGrid(int g[25][40]);
    void setCurrentPlayerTotalPoints(int p) { currentPlayerTotalPoints = p; }
    void setBonusMultiplierCount(int b) { bonusMultiplierCount = b; }
    void setPowerUpCount(int p) { powerUpCount = p; }
    void setPowerUpActive(bool p) { powerUpActive = p; }
    void setPowerUpTimeRemaining(float t) { powerUpTimeRemaining = t; }
    void setEnemyCount(int count) { enemyCount = count; }
    void setEnemy(int index, int x, int y, int dx, int dy);
    void setDifficulty(int minSpd, int maxSpd) { minSpeed = minSpd; maxSpeed = maxSpd; }
    
    
    string getSaveID() const { return saveID; }
    time_t getTimestamp() const { return timestamp; }
    string getUsername() const { return username; }
    int getPlayerID() const { return playerID; }
    float getGameTimer() const { return gameTimer; }
    int getTotalPointsEarnedThisGame() const { return totalPointsEarnedThisGame; }
    void getPlayerPosition(int& x, int& y) const { x = playerX; y = playerY; }
    void getGrid(int g[25][40]) const;
    int getCurrentPlayerTotalPoints() const { return currentPlayerTotalPoints; }
    int getBonusMultiplierCount() const { return bonusMultiplierCount; }
    int getPowerUpCount() const { return powerUpCount; }
    bool getPowerUpActive() const { return powerUpActive; }
    float getPowerUpTimeRemaining() const { return powerUpTimeRemaining; }
    int getEnemyCount() const { return enemyCount; }
    SavedEnemy getEnemy(int index) const { return enemies[index]; }
    void getDifficulty(int& minSpd, int& maxSpd) const { minSpd = minSpeed; maxSpd = maxSpeed; }
};