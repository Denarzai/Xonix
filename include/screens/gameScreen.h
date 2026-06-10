#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../core/player.h"
#include "../core/leaderboard.h"
#include "../core/difficulty.h"
#include "pauseScreen.h"
#include "endGameScreen.h"
#include "../core/saveGameManager.h"
#include "../core/saveGame.h"
#include "../core/themeInventory.h"

struct Enemy {
    int x, y;
    int dx, dy;

    Enemy(int minSpeed = 0, int maxSpeed = 0) {
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
        if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }

        y += dy; 
        if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
    }
};


class GameScreen {
private:
    const int M = 25;
    const int N = 40;
    int grid[25][40];
    int ts = 18;
    
    sf::Texture t1, t2, t3;
    sf::Sprite sTile, sGameover, sEnemy;
    
    int enemyCount;
    int minSpeed;
    int maxSpeed;
    Enemy enemies[10];

    
    bool gameRunning;
    int playerX, playerY, dx, dy;
    float timer, delay;
    sf::Clock clock;
    float gameTimer = 0.0f; 
    bool gamePaused;
    
    Player* currentPlayer;
    PlayerManager* playerManager;
    Leaderboard* leaderboard;
    sf::Font font;
    Difficulty* difficulty;

    ThemeInventory* themeInventory;
    Theme* currentTheme;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    int windowWidth;
    int windowHeight;
    
    
    int pointsEarnedThisMove;
    int totalPointsEarnedThisGame;
    bool playerWasMoving;
    int previousX, previousY;

    const int GAME_START_X = 240;      
    const int GAME_START_Y = 100;      
    const int HUD_START_X = 980;      
    const int CONTROLS_Y = 480; 

    SaveGameManager* saveGameManager;

    bool isLoadedGame;
    string loadedSaveID;
    int pointsBeforeLoad;
    
public:
    GameScreen(PlayerManager* pm, Player* player, Leaderboard* lb, Difficulty *diff, ThemeInventory* ti, int width, int height);
    ~GameScreen();
    
    void run(sf::RenderWindow& window, bool& backToMenu, PauseScreen* pauseScreen, EndGameScreen* endScreen);
    void handleInput(sf::RenderWindow& window, bool& backToMenu);
    void update();
    void draw(sf::RenderWindow& window);
    
    
    int getTotalPointsEarnedThisGame() const { return totalPointsEarnedThisGame; }
    int getPointsEarnedThisMove() const { return pointsEarnedThisMove; }
    int countCapturedTiles() const;
    bool getIsLoadedGame() const { return isLoadedGame; }
    string getLoadedSaveID() const { return loadedSaveID; }

    void saveCurrentGame();
    void loadGameFromSave(const class SaveGame& save);
    
private:
    void initializeGrid();
    void drop(int y, int x);
    void resetGame();
    void countTilesInMove();  
    void awardPointsForMove(int capturedBefore);  
    void drawHUD(sf::RenderWindow& window);  
    void applyDifficulty();

    void drawGameArea(sf::RenderWindow& window);     
    void drawPlayerHUD(sf::RenderWindow& window);    
    void drawControlInstructions(sf::RenderWindow& window);  
    void drawTimer(sf::RenderWindow& window) ;
};

#endif 