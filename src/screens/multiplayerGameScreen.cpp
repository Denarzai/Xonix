#include "../../include/screens/multiplayerGameScreen.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

MultiplayerGameScreen::MultiplayerGameScreen(PlayerManager* pm, Player* p1, Player* p2, 
                                             Leaderboard* lb, Difficulty* diff,
                                             ThemeInventory* ti, int width, int height)
    : playerManager(pm), player1(p1), player2(p2), leaderboard(lb), difficulty(diff),
      themeInventory(ti), windowWidth(width), windowHeight(height), gameRunning(true), gamePaused(false),
      gameTimer(0.0f), enemyCount(0), minSpeed(0), maxSpeed(0) {
    
    multiplayerGame = nullptr;
    
    
    currentTheme = themeInventory->getCurrentTheme();
    
    
    if (!t1.loadFromFile(currentTheme->getTilesImagePath())) {
        std::cerr << "Error loading tiles from: " << currentTheme->getTilesImagePath() << std::endl;
    }
    if (!t2.loadFromFile("images/gameover.png")) {
        std::cerr << "Error loading gameover.png!" << std::endl;
    }
    if (!t3.loadFromFile(currentTheme->getEnemyImagePath())) {
        std::cerr << "Error loading enemy from: " << currentTheme->getEnemyImagePath() << std::endl;
    }
    
    sTile.setTexture(t1);
    sGameover.setTexture(t2);
    sEnemy.setTexture(t3);
    
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    if (!backgroundTexture.loadFromFile(currentTheme->getBackgroundImagePath())) {
        std::cerr << "Error loading background from: " << currentTheme->getBackgroundImagePath() << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        (float)windowWidth / backgroundTexture.getSize().x,
        (float)windowHeight / backgroundTexture.getSize().y
    );
    backgroundSprite.setPosition(0, 0);


    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    
    applyDifficulty();
    initializeGame();
}

MultiplayerGameScreen::~MultiplayerGameScreen() {
    if (multiplayerGame) delete multiplayerGame;
}

void MultiplayerGameScreen::applyDifficulty() {
    switch (*difficulty) {
        case Difficulty::EASY:
            enemyCount = 2;
            minSpeed = 0;
            maxSpeed = 1;
            break;
        case Difficulty::MEDIUM:
            enemyCount = 3;
            minSpeed = 6;
            maxSpeed = 6;
            break;
        case Difficulty::HARD:
            enemyCount = 4;
            minSpeed = 2;
            maxSpeed = 3;
            break;
    }
}

void MultiplayerGameScreen::initializeGame() {
    if (multiplayerGame) delete multiplayerGame;
    
    multiplayerGame = new Multiplayer(player1, player2, enemyCount, minSpeed, maxSpeed);
    multiplayerGame->initialize();
    
    gameRunning = true;
    gamePaused = false;
    gameTimer = 0.0f;
    clock.restart();
}

void MultiplayerGameScreen::handleInput(sf::RenderWindow& window, bool& backToMenu) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            backToMenu = true;
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                gamePaused = true;
            }

            if (event.key.code == sf::Keyboard::G) {
                multiplayerGame->usePowerUpPlayer1();
            }
            if (event.key.code == sf::Keyboard::P) {
                multiplayerGame->usePowerUpPlayer2();
            }
        }
    }
    
    int p1dx = 0, p1dy = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) p1dx = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) p1dx = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) p1dy = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) p1dy = 1;
    
    if (p1dx != 0 || p1dy != 0) {
        multiplayerGame->handlePlayer1Input(p1dx, p1dy);
    }

    int p2dx = 0, p2dy = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p2dx = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p2dx = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    p2dy = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  p2dy = 1;
    
    if (p2dx != 0 || p2dy != 0) {
        multiplayerGame->handlePlayer2Input(p2dx, p2dy);
    }
}

void MultiplayerGameScreen::update() {
    float deltaTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    
    gameTimer += deltaTime;
    multiplayerGame->update(deltaTime);
    
    if (!multiplayerGame->isGameRunning()) {
        gameRunning = false;
    }
}

void MultiplayerGameScreen::drawGameArea(sf::RenderWindow& window) {
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int gridValue = multiplayerGame->getGrid(i, j);
            
            if (gridValue == 0) continue;  
            
            if (gridValue == 1 || gridValue == 4 || gridValue == 5) {
                sTile.setTextureRect(sf::IntRect(0, 0, ts, ts));  
            } 
            else if (gridValue == 2) {
                sTile.setTextureRect(sf::IntRect(54, 0, ts, ts));  
            } 
            else if (gridValue == 3) {
                sTile.setTextureRect(sf::IntRect(72, 0, ts, ts));  
            }
            
            sTile.setPosition(GAME_START_X + j * ts, GAME_START_Y + i * ts);
            window.draw(sTile);
        }
    }
    
    
    sTile.setTextureRect(sf::IntRect(36, 0, ts, ts));
    sTile.setPosition(GAME_START_X + multiplayerGame->getP1X() * ts, 
                     GAME_START_Y + multiplayerGame->getP1Y() * ts);
    window.draw(sTile);
    
    
    sTile.setTextureRect(sf::IntRect(108, 0, ts, ts));
    sTile.setPosition(GAME_START_X + multiplayerGame->getP2X() * ts, 
                     GAME_START_Y + multiplayerGame->getP2Y() * ts);
    window.draw(sTile);
    
    
    sEnemy.rotate(10);
    MultiplayerEnemy* enemies = multiplayerGame->getEnemies();
    for (int i = 0; i < multiplayerGame->getEnemyCount(); i++) {
        sEnemy.setPosition(GAME_START_X + enemies[i].x, GAME_START_Y + enemies[i].y);
        window.draw(sEnemy);
    }
    
    
    if (!multiplayerGame->isGameRunning()) {
        sGameover.setPosition(GAME_START_X + 100, GAME_START_Y + 100);
        window.draw(sGameover);
    }
}

void MultiplayerGameScreen::drawPlayer1HUD(sf::RenderWindow& window, int hudX) {
    if (!player1) return;
    
    int hudY = GAME_START_Y;
    int lineSpacing = 35;
    
    sf::Text hudTitle("PLAYER 1", font, 24);
    hudTitle.setFillColor(sf::Color::Cyan);
    hudTitle.setStyle(sf::Text::Bold);
    hudTitle.setPosition(hudX, hudY);
    window.draw(hudTitle);
    
    sf::RectangleShape separator(sf::Vector2f(200, 2));
    separator.setFillColor(sf::Color::White);
    separator.setPosition(hudX, hudY + 35);
    window.draw(separator);
    
    hudY += 50;

    sf::Text playerText("Player: " + player1->getUsername(), font, 18);
    playerText.setFillColor(sf::Color::White);
    playerText.setPosition(hudX, hudY);
    window.draw(playerText);
    hudY += lineSpacing;
    
    sf::Text totalScoreText("Total Score: " + std::to_string(player1->getTotalPoints()), font, 16);
    totalScoreText.setFillColor(sf::Color::White);
    totalScoreText.setPosition(hudX, hudY);
    window.draw(totalScoreText);
    hudY += lineSpacing;
    
    sf::Text gameScoreText("Game Score: " + std::to_string(multiplayerGame->getP1TotalPointsThisGame()), font, 16);
    gameScoreText.setFillColor(sf::Color::White);
    gameScoreText.setPosition(hudX, hudY);
    window.draw(gameScoreText);
    hudY += lineSpacing;

    int tiles = multiplayerGame->countP1CapturedTiles();
    sf::Text tilesText("Tiles Captured: " + std::to_string(tiles), font, 16);
    tilesText.setFillColor(sf::Color::White);
    tilesText.setPosition(hudX, hudY);
    window.draw(tilesText);
    hudY += lineSpacing;
    
    
    sf::Text statusText(multiplayerGame->isPlayer1Alive() ? "STATUS: ALIVE" : "STATUS: DEAD", font, 16);
    statusText.setFillColor(multiplayerGame->isPlayer1Alive() ? sf::Color::Green : sf::Color::Red);
    statusText.setStyle(sf::Text::Bold);
    statusText.setPosition(hudX, hudY);
    window.draw(statusText);
    hudY += lineSpacing;
    
    
    separator.setPosition(hudX, hudY );
    window.draw(separator);
    hudY += 15;
    
    sf::Text powerUpText("Power-ups: " + std::to_string(player1->getPowerUps()), font, 16);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setPosition(hudX, hudY);
    window.draw(powerUpText);
    hudY += lineSpacing;
    
    if (player1->isPowerUpActive()) {
        sf::Text activeText("ACTIVE: " + std::to_string((int)player1->getPowerUpTimeRemaining()) + "s", font, 14);
        activeText.setFillColor(sf::Color::Red);
        activeText.setStyle(sf::Text::Bold);
        activeText.setPosition(hudX, hudY);
        window.draw(activeText);
    } 
    else {
        sf::Text infoText("(Press G to use)", font, 14);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(hudX, hudY);
        window.draw(infoText);
    }
    hudY += lineSpacing + 5;

    separator.setPosition(hudX, hudY - 10);
    window.draw(separator);
    hudY += 10;

    sf::Text instructTitle("CONTROLS", font, 18);
    instructTitle.setFillColor(sf::Color::Yellow);
    instructTitle.setPosition(hudX, hudY);
    window.draw(instructTitle);
    hudY += lineSpacing;
    
    sf::Text moveText("WASD Keys - Move", font, 14);
    moveText.setFillColor(sf::Color::White);
    moveText.setPosition(hudX, hudY);
    window.draw(moveText);
    hudY += lineSpacing;
    
    sf::Text powerText("G - Use Power-up", font, 14);
    powerText.setFillColor(sf::Color::White);
    powerText.setPosition(hudX, hudY);
    window.draw(powerText);
    hudY += lineSpacing;
}


void MultiplayerGameScreen::drawPlayer2HUD(sf::RenderWindow& window, int hudX) {
    if (!player2) return;
    
    int hudY = GAME_START_Y;
    int lineSpacing = 35;
    
    sf::Text hudTitle("PLAYER 2", font, 24);
    hudTitle.setFillColor(sf::Color::Cyan);
    hudTitle.setStyle(sf::Text::Bold);
    hudTitle.setPosition(hudX, hudY);
    window.draw(hudTitle);
    
    sf::RectangleShape separator(sf::Vector2f(200, 2));
    separator.setFillColor(sf::Color::White);
    separator.setPosition(hudX, hudY + 35);
    window.draw(separator);
    
    hudY += 50;

    sf::Text playerText("Player: " + player2->getUsername(), font, 18);
    playerText.setFillColor(sf::Color::White);
    playerText.setPosition(hudX, hudY);
    window.draw(playerText);
    hudY += lineSpacing;
    
    sf::Text totalScoreText("Total Score: " + std::to_string(player2->getTotalPoints()), font, 16);
    totalScoreText.setFillColor(sf::Color::White);
    totalScoreText.setPosition(hudX, hudY);
    window.draw(totalScoreText);
    hudY += lineSpacing;
    
    sf::Text gameScoreText("Game Score: " + std::to_string(multiplayerGame->getP2TotalPointsThisGame()), font, 16);
    gameScoreText.setFillColor(sf::Color::White);
    gameScoreText.setPosition(hudX, hudY);
    window.draw(gameScoreText);
    hudY += lineSpacing;

    int tiles = multiplayerGame->countP2CapturedTiles();
    sf::Text tilesText("Tiles Captured: " + std::to_string(tiles), font, 16);
    tilesText.setFillColor(sf::Color::White);
    tilesText.setPosition(hudX, hudY);
    window.draw(tilesText);
    hudY += lineSpacing;
    
    
    sf::Text statusText(multiplayerGame->isPlayer2Alive() ? "STATUS: ALIVE" : "STATUS: DEAD", font, 16);
    statusText.setFillColor(multiplayerGame->isPlayer2Alive() ? sf::Color::Green : sf::Color::Red);
    statusText.setStyle(sf::Text::Bold);
    statusText.setPosition(hudX, hudY);
    window.draw(statusText);
    hudY += lineSpacing;
    
    
    separator.setPosition(hudX, hudY );
    window.draw(separator);
    hudY += 15;
    
    sf::Text powerUpText("Power-ups: " + std::to_string(player2->getPowerUps()), font, 16);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setPosition(hudX, hudY);
    window.draw(powerUpText);
    hudY += lineSpacing;
    
    if (player2->isPowerUpActive()) {
        sf::Text activeText("ACTIVE: " + std::to_string((int)player2->getPowerUpTimeRemaining()) + "s", font, 14);
        activeText.setFillColor(sf::Color::Red);
        activeText.setStyle(sf::Text::Bold);
        activeText.setPosition(hudX, hudY);
        window.draw(activeText);
    } 
    else {
        sf::Text infoText("(Press P to use)", font, 14);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(hudX, hudY);
        window.draw(infoText);
    }
    hudY += lineSpacing + 5;

    separator.setPosition(hudX, hudY - 10);
    window.draw(separator);
    hudY += 10;

    sf::Text instructTitle("CONTROLS", font, 18);
    instructTitle.setFillColor(sf::Color::Yellow);
    instructTitle.setPosition(hudX, hudY);
    window.draw(instructTitle);
    hudY += lineSpacing;
    
    sf::Text moveText("Arrow Keys - Move", font, 14);
    moveText.setFillColor(sf::Color::White);
    moveText.setPosition(hudX, hudY);
    window.draw(moveText);
    hudY += lineSpacing;
    
    sf::Text powerText("P - Use Power-up", font, 14);
    powerText.setFillColor(sf::Color::White);
    powerText.setPosition(hudX, hudY);
    window.draw(powerText);
    hudY += lineSpacing;
}

void MultiplayerGameScreen::drawDualHUD(sf::RenderWindow& window) {
    drawPlayer1HUD(window, HUD_P1_START_X);
    drawPlayer2HUD(window, HUD_P2_START_X);
}

void MultiplayerGameScreen::drawTimer(sf::RenderWindow& window) {
    int totalSeconds = static_cast<int>(multiplayerGame->getGameTimer());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    
    std::string timeStr = (minutes < 10 ? "0" : "") + std::to_string(minutes)
                        + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    
    sf::Text timerText(timeStr, font, 50);
    timerText.setFillColor(sf::Color::White);
    
    float textWidth = timerText.getLocalBounds().width;
    timerText.setPosition(windowWidth / 2 - textWidth / 2, 25);
    
    window.draw(timerText);
}

void MultiplayerGameScreen::drawFreezeIndicator(sf::RenderWindow& window) {
    if (multiplayerGame->areEnemiesFrozen()) {
        sf::Text freezeText("FROZEN!", font, 24);
        freezeText.setFillColor(sf::Color::Cyan);
        freezeText.setStyle(sf::Text::Bold);
        freezeText.setPosition(windowWidth / 2 - 60, windowHeight / 2);
        window.draw(freezeText);
    }
}

void MultiplayerGameScreen::drawControlInstructions(sf::RenderWindow& window) {
    int instructionsY = CONTROLS_Y;
    int startX = GAME_START_X;
    
    sf::Text escapeText("Escape - Pause", font, 30);
    escapeText.setFillColor(sf::Color::White);
    startX = (window.getSize().x - escapeText.getLocalBounds().width) / 2;
    instructionsY = window.getSize().y - 65;
    escapeText.setPosition(startX, instructionsY);
    window.draw(escapeText);
}

void MultiplayerGameScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(backgroundSprite);
    drawGameArea(window);
    drawDualHUD(window);
    drawTimer(window);
    drawFreezeIndicator(window);
    drawControlInstructions(window);
    
    window.display();
}

Player* MultiplayerGameScreen::getWinner() const {
    return multiplayerGame->getWinner();
}

void MultiplayerGameScreen::run(sf::RenderWindow& window, bool& backToMenu, 
                               MultiplayerPauseScreen* pauseScreen, MultiplayerEndGameScreen* endScreen) {
    initializeGame();
    
    while (window.isOpen() && !backToMenu && gameRunning) {
        handleInput(window, backToMenu);
        
        if (gamePaused) {
            int pauseChoice = -1;
            pauseScreen->run(window, pauseChoice);
            clock.restart();
            
            if (pauseChoice == 0) {  
                gamePaused = false;
            } 
            else if (pauseChoice == 1) {  
                backToMenu = true;
                break;
            }
        }
        
        update();
        draw(window);
    }

    if (!gameRunning && !backToMenu && window.isOpen()) {
        int endChoice = -1;
        
        Player* winner = getWinner();
        int p1GameScore = multiplayerGame->getP1TotalPointsThisGame();
        int p2GameScore = multiplayerGame->getP2TotalPointsThisGame();

        player1->addPoints(p1GameScore);
        player2->addPoints(p2GameScore);
        
        
        
        
        
        
        endScreen->run(window, endChoice, p1GameScore, p2GameScore);
        
        if (endChoice == 0) { 
            gameRunning = true;
            run(window, backToMenu, pauseScreen, endScreen);
        } 
        else if (endChoice == 1) {  
            backToMenu = true;
        }
    }
}