#include "../../include/core/multiplayer.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

Multiplayer::Multiplayer(Player* p1, Player* p2, int enemyCount, int minSpd, int maxSpd)
    : player1(p1), player2(p2), enemyCount(enemyCount), minSpeed(minSpd), maxSpeed(maxSpd),
      gameRunning(true), player1Alive(true), player2Alive(true),
      p1X(10), p1Y(0), p1Dx(0), p1Dy(0),
      p2X(30), p2Y(0), p2Dx(0), p2Dy(0),
      timer(0), delay(0.07f), gameTimer(0.0f),
      p1PointsThisGame(0), p2PointsThisGame(0),
      p1PointsThisMove(0), p2PointsThisMove(0),
      p1WasMoving(false), p2WasMoving(false),
      p1PreviousX(p1X), p1PreviousY(p1Y),
      p2PreviousX(p2X), p2PreviousY(p2Y),
      enemiesFrozen(false), freezeTimeRemaining(0.0f), player1Frozen(false), player2Frozen(false) {
    
    initializeGrid();
    resetGameState();
}

Multiplayer::~Multiplayer() {
    
}

void Multiplayer::initializeGrid() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                grid[i][j] = 1;
            }
        }
    }
}

void Multiplayer::resetGameState() {
    initializeGrid();
    
    p1X = 10;
    p1Y = 0;
    p1Dx = 0;
    p1Dy = 0;
    
    p2X = 30;
    p2Y = 0;
    p2Dx = 0;
    p2Dy = 0;
    
    gameRunning = true;
    player1Alive = true;
    player2Alive = true;
    timer = 0;
    gameTimer = 0.0f;
    
    p1PointsThisGame = 0;
    p2PointsThisGame = 0;
    p1PointsThisMove = 0;
    p2PointsThisMove = 0;
    
    p1WasMoving = false;
    p2WasMoving = false;
    
    enemiesFrozen = false;
    freezeTimeRemaining = 0.0f;
    player1Frozen = false;
    player2Frozen = false;
    
    
    if (player1) {
        player1->resetMoveCounter();
        player1->setTilesCapturedInCurrentMove(0);
        player1->setBonusMultiplierCount(0);
        player1->updateMultiplier();
        player1->deactivatePowerUp();
    }
    
    if (player2) {
        player2->resetMoveCounter();
        player2->setTilesCapturedInCurrentMove(0);
        player2->setBonusMultiplierCount(0);
        player2->updateMultiplier();
        player2->deactivatePowerUp();
    }
    
    
    for (int i = 0; i < 10; i++) {
        if (i < enemyCount) {
            MultiplayerEnemy e(minSpeed, maxSpeed);
            
            bool overlaps = false;
            for (int j = 0; j < i; j++) {
                if (j >= enemyCount) continue;
                
                if (abs(e.x - enemies[j].x) < ts && abs(e.y - enemies[j].y) < ts) {
                    overlaps = true;
                    break;
                }
                
                if (e.dx == enemies[j].dx && e.dy == enemies[j].dy) {
                    overlaps = true;
                    break;
                }
            }
            
            if (overlaps) {
                i--;
                continue;
            }
            
            enemies[i] = e;
        } 
        else {
            enemies[i] = MultiplayerEnemy(0, 0);
        }
    }
    
    clock.restart();
}

void Multiplayer::initialize() {
    resetGameState();
}

int Multiplayer::countP1CapturedTiles() const {
    int count = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 4) {  
                count++;
            }
        }
    }
    return count;
}

int Multiplayer::countP2CapturedTiles() const {
    int count = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 5) {  
                count++;
            }
        }
    }
    return count;
}

void Multiplayer::dropP1(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    
    
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) dropP1(y - 1, x);
    if (y + 1 < M && grid[y + 1][x] == 0) dropP1(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) dropP1(y, x - 1);
    if (x + 1 < N && grid[y][x + 1] == 0) dropP1(y, x + 1);
}


void Multiplayer::dropP2(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    
    
    if (grid[y][x] == 0) grid[y][x] = -2;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) dropP2(y - 1, x);
    if (y + 1 < M && grid[y + 1][x] == 0) dropP2(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) dropP2(y, x - 1);
    if (x + 1 < N && grid[y][x + 1] == 0) dropP2(y, x + 1);
}

/*
void Multiplayer::drop(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) drop(y - 1, x);
    if (y + 1 < M && grid[y + 1][x] == 0) drop(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) drop(y, x - 1);
    if (x + 1 < N && grid[y][x + 1] == 0) drop(y, x + 1);
}
*/

bool Multiplayer::checkPlayerCollision(int x1, int y1, int x2, int y2) const {
    return (x1 == x2 && y1 == y2);
}

bool Multiplayer::checkPlayerTileCollision(int playerX, int playerY, int gridValue) {
    return grid[playerY][playerX] == gridValue;
}

void Multiplayer::handlePlayerCollision() {
    
    if (checkPlayerCollision(p1X, p1Y, p2X, p2Y)) {
        bool p1Moving = (p1Dx != 0 || p1Dy != 0);
        bool p2Moving = (p2Dx != 0 || p2Dy != 0);
        
        if (p1Moving && p2Moving) {
            
            player1Alive = false;
            player2Alive = false;
        } else if (p1Moving && !p2Moving) {
            
            player1Alive = false;
        } else if (p2Moving && !p1Moving) {
            
            player2Alive = false;
        }
    }
    
    
    if (player1Alive && grid[p1Y][p1X] == 3 && !checkPlayerCollision(p1X, p1Y, p2X, p2Y)) {
        player1Alive = false;
    }
    
    
    if (player2Alive && grid[p2Y][p2X] == 2 && !checkPlayerCollision(p1X, p1Y, p2X, p2Y)) {
        player2Alive = false;
    }
}

void Multiplayer::handleEnemyCollision() {
    
    for (int i = 0; i < enemyCount; i++) {
        int enemyGridX = enemies[i].x / ts;
        int enemyGridY = enemies[i].y / ts;
        
        if ((enemyGridX == p1X && enemyGridY == p1Y) || (grid[enemyGridY][enemyGridX] == 2)) {
            player1Alive = false;
            
        }
        
        if ((enemyGridX == p2X && enemyGridY == p2Y) || (grid[enemyGridY][enemyGridX] == 3)) {
            player2Alive = false;
            
        }
    }
}

void Multiplayer::awardPointsForMoveP1(int capturedBefore) {
    if (!player1) return;
    
    int threshold = (player1->getBonusMultiplierCount() >= 3) ? 5 : 10;
    int capturedNow = countP1CapturedTiles() - capturedBefore;
    
    player1->setTilesCapturedInCurrentMove(capturedNow);
    
    if (capturedNow > threshold) {
        player1->setBonusMultiplierCount(player1->getBonusMultiplierCount() + 1);
    }
    
    player1->updateMultiplier();
    int multiplier = player1->getCurrentMultiplier();
    p1PointsThisMove = capturedNow * multiplier;
    
    if (p1PointsThisMove > 0) {
        p1PointsThisGame += p1PointsThisMove;
        player1->awardPoints(p1PointsThisMove, p1PointsThisGame);
    }
    
    player1->resetMoveCounter();
    p1WasMoving = false;
    p1PointsThisMove = 0;
}

void Multiplayer::awardPointsForMoveP2(int capturedBefore) {
    if (!player2) return;    
    
    int threshold = (player2->getBonusMultiplierCount() >= 3) ? 5 : 10;
    int capturedNow = countP2CapturedTiles() - capturedBefore;
    
    player2->setTilesCapturedInCurrentMove(capturedNow);
    
    if (capturedNow > threshold) {
        player2->setBonusMultiplierCount(player2->getBonusMultiplierCount() + 1);
    }
    
    player2->updateMultiplier();
    int multiplier = player2->getCurrentMultiplier();
    p2PointsThisMove = capturedNow * multiplier;
    
    if (p2PointsThisMove > 0) {
        p2PointsThisGame += p2PointsThisMove;
        player2->awardPoints(p2PointsThisMove, p2PointsThisGame);
    }
    
    player2->resetMoveCounter();
    p2WasMoving = false;
    p2PointsThisMove = 0;
}

void Multiplayer::updateFreezeTimer(float deltaTime) {
    if (enemiesFrozen) {
        freezeTimeRemaining -= deltaTime;
        if (freezeTimeRemaining <= 0.0f) {
            enemiesFrozen = false;
            freezeTimeRemaining = 0.0f;
            player1Frozen = false;
            player2Frozen = false;
        }
    }
}

void Multiplayer::activatePowerUp(Player* activator) {
    if (!activator || activator->getPowerUps() <= 0) return;
    
    activator->usePowerUp();
    enemiesFrozen = true;
    freezeTimeRemaining = FREEZE_DURATION;

    if (activator == player1) {
        player2Frozen = true;
        player1Frozen = false;
    } 
    else {
        player1Frozen = true;
        player2Frozen = false;
    }
}

void Multiplayer::handlePlayer1Input(int dx, int dy) {
    p1Dx = dx;
    p1Dy = dy;
    p1WasMoving = (dx != 0 || dy != 0);
}

void Multiplayer::handlePlayer2Input(int dx, int dy) {
    p2Dx = dx;
    p2Dy = dy;
    p2WasMoving = (dx != 0 || dy != 0);
}

void Multiplayer::usePowerUpPlayer1() {
    activatePowerUp(player1);
}

void Multiplayer::usePowerUpPlayer2() {
    activatePowerUp(player2);
}

void Multiplayer::update(float deltaTime) {
    gameTimer += deltaTime;
    updateFreezeTimer(deltaTime);
    
    if (!gameRunning) return;
    
    if (player1) player1->updatePowerUpTimer(deltaTime);
    if (player2) player2->updatePowerUpTimer(deltaTime);
    
    
    if (!player1Alive && !player2Alive) {
        gameRunning = false;
        return;
    }

    if(!player1Alive) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 2) grid[i][j] = 0;
            }
        }
        p1X = GAME_START_X * ts;
        p1Y = GAME_START_Y * ts;
    }
    if(!player2Alive) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 3) grid[i][j] = 0;
            }
        }
        p2X = GAME_START_X * ts;
        p2Y = GAME_START_Y * ts;
    }
    
    timer += deltaTime;
    
    if (timer > delay) {
        
        if (player1Alive && p1WasMoving && !player1Frozen) {
            p1PreviousX = p1X;
            p1PreviousY = p1Y;
            
            p1X += p1Dx;
            p1Y += p1Dy;
            
            if (p1X < 0) p1X = 0;
            if (p1X > N - 1) p1X = N - 1;
            if (p1Y < 0) p1Y = 0;
            if (p1Y > M - 1) p1Y = M - 1;
            
            
            if (grid[p1Y][p1X] == 1 || grid[p1Y][p1X] == 3 || grid[p1Y][p1X] == 4 || grid[p1Y][p1X] == 5) {
                
                if (grid[p1Y][p1X] == 3) {
                    player1Alive = false;
                } 
                else {
                    
                    int capturedBefore = countP1CapturedTiles();
                    //cout << "Tiles captured befre: " << capturedBefore << endl << endl;
                    p1Dx = p1Dy = 0;
                    
                    for (int i = 0; i < enemyCount; i++) {
                        dropP1(enemies[i].y / ts, enemies[i].x / ts);
                    }
                    
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == -1) grid[i][j] = 0;
                            else if (grid[i][j] != 1 && grid[i][j] != 3 && grid[i][j] != 5) {  
                                grid[i][j] = 4;  
                            }
                        }
                    }
                    
                    awardPointsForMoveP1(capturedBefore);
                }
            } 
            else if (grid[p1Y][p1X] == 2) {
                player1Alive = false;
            }
            else if (grid[p1Y][p1X] == 0) {
                
                grid[p1Y][p1X] = 2;
            }
        }
        
        
        if (player2Alive && p2WasMoving && !player2Frozen) {
            p2PreviousX = p2X;
            p2PreviousY = p2Y;
            
            p2X += p2Dx;
            p2Y += p2Dy;
            
            
            if (p2X < 0) p2X = 0;
            if (p2X > N - 1) p2X = N - 1;
            if (p2Y < 0) p2Y = 0;
            if (p2Y > M - 1) p2Y = M - 1;
            
            
            if (grid[p2Y][p2X] == 1 || grid[p2Y][p2X] == 2 || grid[p2Y][p2X] == 4 || grid[p2Y][p2X] == 5) {
                
                if (grid[p2Y][p2X] == 2) {
                    player2Alive = false;
                } 
                else {
                    
                    int capturedBefore = countP2CapturedTiles();
                    p2Dx = p2Dy = 0;
                    
                    for (int i = 0; i < enemyCount; i++) {
                        dropP2(enemies[i].y / ts, enemies[i].x / ts);
                    }
                    
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == -2) grid[i][j] = 0;
                            else if (grid[i][j] != 1 && grid[i][j] != 2 && grid[i][j] != 4) {  
                                grid[i][j] = 5;  
                            }
                        }
                    }
                    
                    awardPointsForMoveP2(capturedBefore);
                }
            } 
            else if (grid[p2Y][p2X] == 3) {
                player2Alive = false;
            }
            else if (grid[p2Y][p2X] == 0) {
                
                grid[p2Y][p2X] = 3;
            }
        }
        
        
        handlePlayerCollision();
        
        
        if (!enemiesFrozen) {
            for (int i = 0; i < enemyCount; i++) {
                enemies[i].move(grid, ts, M, N);
            }
        }
        
        
        handleEnemyCollision();
        
        
        if (!player1Alive && !player2Alive) {
            gameRunning = false;
        }
        
        timer = 0;
    }
}

Player* Multiplayer::getWinner() const {
    if (!gameRunning && !player1Alive && !player2Alive) {
        int p1Score = player1 ? player1->getTotalPoints() : 0;
        int p2Score = player2 ? player2->getTotalPoints() : 0;
        
        if (p1Score > p2Score) return player1;
        if (p2Score > p1Score) return player2;
        return nullptr;  
    }
    return nullptr;
}