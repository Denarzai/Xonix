#pragma once
#include "player.h"
#include <SFML/Graphics.hpp>

struct MultiplayerEnemy {
    int x, y;
    int dx, dy;

    MultiplayerEnemy(int minSpeed = 0, int maxSpeed = 0) {
        x = (2 + rand() % (38)) * 18;
        y = (2 + rand() % (23)) * 18;

        int dirX = (rand() % 2) ? 1 : -1;
        int dirY = (rand() % 2) ? 1 : -1;

        int speedX = minSpeed + rand() % (maxSpeed - minSpeed + 1);
        int speedY = minSpeed + rand() % (maxSpeed - minSpeed + 1);

        dx = dirX * speedX;
        dy = dirY * speedY;
    }

    void move(int grid[25][40], int ts, int M, int N) {
        
        x += dx;
        int tileX = x / ts;
        int tileY = y / ts;

        if (grid[tileY][tileX] == 1 || grid[tileY][tileX] == 4 || grid[tileY][tileX] == 5) {
            dx = -dx;
            x += dx;
        }

        
        y += dy;
        tileX = x / ts;
        tileY = y / ts;

        if (grid[tileY][tileX] == 1 || grid[tileY][tileX] == 4 || grid[tileY][tileX] == 5) {
            dy = -dy;
            y += dy;
        }
    }

};

class Multiplayer {
private:
    const int M = 25;
    const int N = 40;
    int ts = 18;

    const int GAME_START_X = 240;
    const int GAME_START_Y = 100;
    
    
    
    
    
    
    
    
    
    
    int grid[25][40];
    
    Player* player1;
    Player* player2;
    
    
    int p1X, p1Y, p1Dx, p1Dy;
    int p2X, p2Y, p2Dx, p2Dy;
    
    
    bool gameRunning;
    bool player1Alive;
    bool player2Alive;
    
    
    int enemyCount;
    int minSpeed, maxSpeed;
    MultiplayerEnemy enemies[10];
    
    
    float timer, delay;
    sf::Clock clock;
    float gameTimer;
    
    
    int p1PointsThisGame, p2PointsThisGame;
    int p1PointsThisMove, p2PointsThisMove;
    bool p1WasMoving, p2WasMoving;
    int p1PreviousX, p1PreviousY;
    int p2PreviousX, p2PreviousY;
    
    
    bool enemiesFrozen;
    float freezeTimeRemaining;
    const float FREEZE_DURATION = 3.0f;
    bool player1Frozen;
    bool player2Frozen;
    
    
    void initializeGrid();
    void resetGameState();
    void countTilesInMoveP1();
    void countTilesInMoveP2();
    void awardPointsForMoveP1(int capturedBefore);
    void awardPointsForMoveP2(int capturedBefore);
    void dropP1(int y, int x);
    void dropP2(int y, int x);
    
    int countP1Tiles() const;
    int countP2Tiles() const;
    
    
    bool checkPlayerCollision(int x1, int y1, int x2, int y2) const;
    bool checkPlayerTileCollision(int playerX, int playerY, int gridValue);
    void handlePlayerCollision();
    void handleEnemyCollision();
    
    
    void activatePowerUp(Player* activator);
    void updateFreezeTimer(float deltaTime);
    
public:
    Multiplayer(Player* p1, Player* p2, int enemyCount, int minSpd, int maxSpd);
    ~Multiplayer();
    
    void initialize();
    void update(float deltaTime);

    int countP1CapturedTiles() const;
    int countP2CapturedTiles() const;

    bool isPlayer1Frozen() const { return player1Frozen; }
    bool isPlayer2Frozen() const { return player2Frozen; }
    
    int getGridValue(int x, int y) const { return (x >= 0 && x < N && y >= 0 && y < M) ? grid[y][x] : 1; }
    int getGrid(int y, int x) const { return grid[y][x]; }
    int getEnemyCount() const { return enemyCount; }
    MultiplayerEnemy* getEnemies() { return enemies; }
    
    int getP1X() const { return p1X; }
    int getP1Y() const { return p1Y; }
    int getP2X() const { return p2X; }
    int getP2Y() const { return p2Y; }
    
    bool isPlayer1Alive() const { return player1Alive; }
    bool isPlayer2Alive() const { return player2Alive; }
    bool isGameRunning() const { return gameRunning; }
    
    int getP1TotalPointsThisGame() const { return p1PointsThisGame; }
    int getP2TotalPointsThisGame() const { return p2PointsThisGame; }
    int getP1PointsThisMove() const { return p1PointsThisMove; }
    int getP2PointsThisMove() const { return p2PointsThisMove; }
    
    float getGameTimer() const { return gameTimer; }
    bool areEnemiesFrozen() const { return enemiesFrozen; }
    float getFreezeTimeRemaining() const { return freezeTimeRemaining; }
    
    Player* getPlayer1() const { return player1; }
    Player* getPlayer2() const { return player2; }
    
    
    void handlePlayer1Input(int dx, int dy);
    void handlePlayer2Input(int dx, int dy);
    void usePowerUpPlayer1();
    void usePowerUpPlayer2();
    
    
    Player* getWinner() const;
    
};