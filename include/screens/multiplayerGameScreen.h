#pragma once

#include <SFML/Graphics.hpp>
#include "../core/multiplayer.h"
#include "../core/playerManager.h"
#include "../core/leaderboard.h"
#include "../core/difficulty.h"
#include "../core/themeInventory.h"
#include "multiplayerPauseScreen.h"
#include "multiplayerEndGameScreen.h"

class MultiplayerGameScreen {
private:
    const int M = 25;
    const int N = 40;
    int ts = 18;
    
    sf::Texture t1, t2, t3;
    sf::Sprite sTile, sGameover, sEnemy;
    
    Multiplayer* multiplayerGame;
    Player* player1;
    Player* player2;
    PlayerManager* playerManager;
    Leaderboard* leaderboard;
    Difficulty* difficulty;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    ThemeInventory* themeInventory;
    Theme* currentTheme;
    
    sf::Font font;
    
    int windowWidth;
    int windowHeight;
    
    bool gameRunning;
    bool gamePaused;
    sf::Clock clock;
    
    float gameTimer;
    
    
    const int GAME_START_X = 240;
    const int GAME_START_Y = 100;
    const int HUD_P1_START_X = 20;
    const int HUD_P2_START_X = 980;
    const int CONTROLS_Y = 480;
    
    
    int enemyCount;
    int minSpeed;
    int maxSpeed;
    
    void initializeGame();
    void applyDifficulty();
    void handleInput(sf::RenderWindow& window, bool& backToMenu);
    void update();
    void draw(sf::RenderWindow& window);
    
    
    void drawGameArea(sf::RenderWindow& window);
    void drawDualHUD(sf::RenderWindow& window);
    void drawPlayer1HUD(sf::RenderWindow& window, int hudX);
    void drawPlayer2HUD(sf::RenderWindow& window, int hudX);
    void drawTimer(sf::RenderWindow& window);
    void drawFreezeIndicator(sf::RenderWindow& window);
    void drawControlInstructions(sf::RenderWindow& window);
    
public:
    MultiplayerGameScreen(PlayerManager* pm, Player* p1, Player* p2, Leaderboard* lb, 
                         Difficulty* diff, ThemeInventory* ti, int width, int height);
    ~MultiplayerGameScreen();
    
    void run(sf::RenderWindow& window, bool& backToMenu, MultiplayerPauseScreen* pauseScreen, 
             MultiplayerEndGameScreen* endScreen);
    
    Player* getWinner() const;
};