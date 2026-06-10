#include "../../include/screens/gameScreen.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

GameScreen::GameScreen(PlayerManager* pm, Player* player, Leaderboard* lb, Difficulty* diff,
                       ThemeInventory* ti, int width, int height)
    : playerManager(pm), currentPlayer(player), leaderboard(lb), difficulty(diff),
      themeInventory(ti), windowWidth(width), windowHeight(height), 
      gameRunning(true), playerX(0), playerY(0), 
      dx(0), dy(0), timer(0), delay(0.07f), pointsEarnedThisMove(0),
      totalPointsEarnedThisGame(0), playerWasMoving(false),
      previousX(0), previousY(0), gamePaused(false), gameTimer(0.0f),
      isLoadedGame(false), pointsBeforeLoad(0), loadedSaveID("") {
    
    saveGameManager = new SaveGameManager();
    enemyCount = 0;
    maxSpeed = 0;
    minSpeed  = 0; 
    applyDifficulty();
    ts = 18;
    
    
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

    if (!backgroundTexture.loadFromFile(currentTheme->getBackgroundImagePath())) {
        std::cerr << "Error loading background from: " << currentTheme->getBackgroundImagePath() << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        (float)windowWidth / backgroundTexture.getSize().x,
        (float)windowHeight / backgroundTexture.getSize().y
    );
    backgroundSprite.setPosition(0, 0);
    
    sTile.setTexture(t1);
    sGameover.setTexture(t2);
    sEnemy.setTexture(t3);
    
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    
    initializeGrid();
    resetGame();
}

GameScreen::~GameScreen() {
    if (saveGameManager) delete saveGameManager;
}

void GameScreen::initializeGrid() {
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;
    
    
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) 
                grid[i][j] = 1;
}

void GameScreen::resetGame() {
    initializeGrid();
    applyDifficulty();

    playerX = 10;
    playerY = 0;
    dx = 0;
    dy = 0;
    gameRunning = true;
    timer = 0;
    clock.restart();
    
    pointsEarnedThisMove = 0;
    totalPointsEarnedThisGame = 0;
    playerWasMoving = false;
    previousX = playerX;
    previousY = playerY;
    
    if (currentPlayer) {
        currentPlayer->resetMoveCounter();
        currentPlayer->setTilesCapturedInCurrentMove(0);
        currentPlayer->setBonusMultiplierCount(0);
        currentPlayer->updateMultiplier();
        currentPlayer->setLastPowerUpThreshold(0);
        currentPlayer->deactivatePowerUp();
    }

    for (int i = 0; i < 10; i++) {
        if (i < enemyCount) {

            Enemy e(minSpeed, maxSpeed);

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
            enemies[i] = Enemy(0, 0);  
        }
    }

}

int GameScreen::countCapturedTiles() const {
    int count = 0;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (!(i == 0 || j == 0 || i == M - 1 || j == N - 1) && grid[i][j] == 1) 
                count++;

    return count;
}

void GameScreen::drop(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) drop(y - 1, x);
    if (y + 1 < M && grid[y + 1][x] == 0) drop(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) drop(y, x - 1);
    if (x + 1 < N && grid[y][x + 1] == 0) drop(y, x + 1);
}

void GameScreen::applyDifficulty() {
    switch (*difficulty) {
        case Difficulty::EASY:
            enemyCount = 3;
            minSpeed = 0;
            maxSpeed = 2;
            break;

        case Difficulty::MEDIUM:
            enemyCount = 4;
            minSpeed = 2;
            maxSpeed = 3;
            break;

        case Difficulty::HARD:
            enemyCount = 5;
            minSpeed = 3;
            maxSpeed = 4;
            break;
    }
}

/*
void GameScreen::countTilesInMove() {
    if (currentPlayer && (dx != 0 || dy != 0)) {
        
        if (grid[playerY][playerX] == 0) {
            currentPlayer->updateMultiplier();
        }
    }
}*/

void GameScreen::awardPointsForMove(int capturedBefore) {
    if (currentPlayer && playerWasMoving) {
        int threshold = (currentPlayer->getBonusMultiplierCount() >= 3) ? 5 : 10;

        int capturedNow = countCapturedTiles() - capturedBefore;
        currentPlayer->setTilesCapturedInCurrentMove(capturedNow);

        if (capturedNow > threshold) {
            currentPlayer->setBonusMultiplierCount(currentPlayer->getBonusMultiplierCount() + 1);
        } 

        currentPlayer->updateMultiplier();
        int multiplier = currentPlayer->getCurrentMultiplier();
        pointsEarnedThisMove = capturedNow * multiplier;
        
        if (pointsEarnedThisMove > 0) {
            totalPointsEarnedThisGame += pointsEarnedThisMove;
            currentPlayer->awardPoints(pointsEarnedThisMove, totalPointsEarnedThisGame);
        }
        
        currentPlayer->resetMoveCounter();
        playerWasMoving = false;
        pointsEarnedThisMove = 0;
    }
}

void GameScreen::handleInput(sf::RenderWindow& window, bool& backToMenu) {
   
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
            
            if (event.key.code == sf::Keyboard::Space) {
                if (currentPlayer && currentPlayer->getPowerUps() > 0) {
                    currentPlayer->usePowerUp();
                }
            }

            if (event.key.code == sf::Keyboard::Left) { dx=-1; dy=0; playerWasMoving=true; }
            if (event.key.code == sf::Keyboard::Right) { dx=1; dy=0; playerWasMoving=true; }
            if (event.key.code == sf::Keyboard::Up) { dx=0; dy=-1; playerWasMoving=true; }
            if (event.key.code == sf::Keyboard::Down) { dx=0; dy=1; playerWasMoving=true; }
        }
    }    
}

void GameScreen::update() {
    float deltaTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += deltaTime;

    gameTimer += deltaTime;
   
    if (currentPlayer) {
        currentPlayer->updatePowerUpTimer(deltaTime);
    }
    
    if (!gameRunning) {
            leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
            leaderboard->saveToFile("leaderboard.txt");
            playerManager->saveToFile("players_test.txt");
        return;
    }
    
    if (timer > delay) {
        previousX = playerX;
        previousY = playerY;
        
        playerX += dx;
        playerY += dy;
        
        if (playerX < 0) playerX = 0;
        if (playerX > N - 1) playerX = N - 1;
        if (playerY < 0) playerY = 0;
        if (playerY > M - 1) playerY = M - 1;

        if (grid[playerY][playerX] == 2) {
            gameRunning = false;
            leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
            leaderboard->saveToFile("leaderboard.txt");
            playerManager->saveToFile("players_test.txt");
        }
        else if (grid[playerY][playerX] == 0) {
            
            /*if (currentPlayer) {
                currentPlayer->incrementTilesInMove();
            }*/
            grid[playerY][playerX] = 2;
        }
        else if (grid[playerY][playerX] == 1) {
            int capturedBefore = countCapturedTiles();
            
            dx = dy = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(enemies[i].y / ts, enemies[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1) grid[i][j] = 0;
                    else grid[i][j] = 1;

            awardPointsForMove(capturedBefore);
        }

        timer = 0;
    }
    
    if (!currentPlayer || !currentPlayer->isPowerUpActive()) {
        for (int i = 0; i < enemyCount; i++) {
            enemies[i].move(grid, ts, M, N);
        }
    }
    
    for (int i = 0; i < enemyCount; i++)
        if (grid[enemies[i].y / ts][enemies[i].x / ts] == 2) {
            gameRunning = false;
            leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
            leaderboard->saveToFile("leaderboard.txt");
            playerManager->saveToFile("players_test.txt");
        }
}

void GameScreen::drawGameArea(sf::RenderWindow& window) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) continue;
            if (grid[i][j] == 1) sTile.setTextureRect(sf::IntRect(0, 0, ts, ts));
            if (grid[i][j] == 2) sTile.setTextureRect(sf::IntRect(54, 0, ts, ts));
            sTile.setPosition(GAME_START_X + j * ts, GAME_START_Y + i * ts);
            window.draw(sTile);
        }
    }
    
    sTile.setTextureRect(sf::IntRect(36, 0, ts, ts));
    sTile.setPosition(GAME_START_X + playerX * ts, GAME_START_Y + playerY * ts);
    window.draw(sTile);
    
    sEnemy.rotate(10);
    for (int i = 0; i < enemyCount; i++) {
        sEnemy.setPosition(GAME_START_X + enemies[i].x, GAME_START_Y + enemies[i].y);
        window.draw(sEnemy);
    }
    
    if (!gameRunning) {
        sGameover.setPosition(GAME_START_X + 100, GAME_START_Y + 100);
        window.draw(sGameover);
        leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
        leaderboard->saveToFile("leaderboard.txt");
        playerManager->saveToFile("players_test.txt");
    }
}

void GameScreen::drawPlayerHUD(sf::RenderWindow& window) {
    if (!currentPlayer) return;
    
    int hudX = HUD_START_X;
    int hudY = GAME_START_Y;
    int lineSpacing = 35;
    
    sf::Text hudTitle("PLAYER STATS", font, 24);
    hudTitle.setFillColor(sf::Color::Cyan);
    hudTitle.setStyle(sf::Text::Bold);
    hudTitle.setPosition(hudX, hudY);
    window.draw(hudTitle);
    
    sf::RectangleShape separator(sf::Vector2f(250, 2));
    separator.setFillColor(sf::Color::White);
    separator.setPosition(hudX, hudY + 35);
    window.draw(separator);
    
    hudY += 50;
    
    sf::Text playerText("Player: " + currentPlayer->getUsername(), font, 18);
    playerText.setFillColor(sf::Color::White);
    playerText.setPosition(hudX, hudY);
    window.draw(playerText);
    hudY += lineSpacing;
    
    sf::Text scoreText("Total Score: " + std::to_string(currentPlayer->getTotalPoints()), font, 18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(hudX, hudY);
    window.draw(scoreText);
    hudY += lineSpacing;
    
    sf::Text gameScoreText("Game Score: " + std::to_string(totalPointsEarnedThisGame), font, 18);
    gameScoreText.setFillColor(sf::Color::White);
    gameScoreText.setPosition(hudX, hudY);
    window.draw(gameScoreText);
    hudY += lineSpacing;
    
    int tiles = countCapturedTiles();
    sf::Text tilesText("Tiles Captured: " + std::to_string(tiles), font, 16);
    tilesText.setFillColor(sf::Color::White);
    tilesText.setPosition(hudX, hudY);
    window.draw(tilesText);
    hudY += lineSpacing;
   
    separator.setPosition(hudX, hudY - 10);
    window.draw(separator);
    hudY += 15;
    
    sf::Text powerTitle("POWER-UPS", font, 18);
    powerTitle.setFillColor(sf::Color::Cyan);
    powerTitle.setPosition(hudX, hudY);
    window.draw(powerTitle);
    hudY += lineSpacing;
    
    sf::Text powerUpText("Available: " + std::to_string(currentPlayer->getPowerUps()), font, 16);
    powerUpText.setFillColor(sf::Color::White);
    powerUpText.setPosition(hudX, hudY);
    window.draw(powerUpText);
    hudY += lineSpacing;
    
    if (currentPlayer->isPowerUpActive()) {
        sf::Text activeText("ACTIVE! Time: " + 
                           std::to_string((int)currentPlayer->getPowerUpTimeRemaining()) + "s", 
                           font, 14);
        activeText.setFillColor(sf::Color::Red);
        activeText.setStyle(sf::Text::Bold);
        activeText.setPosition(hudX, hudY);
        window.draw(activeText);
    } 
    else {
        sf::Text inactiveText("(Press Space to use)", font, 14);
        inactiveText.setFillColor(sf::Color::White);
        inactiveText.setPosition(hudX, hudY);
        window.draw(inactiveText);
    }
    hudY += lineSpacing;

    separator.setPosition(hudX, hudY - 10);
    window.draw(separator);
    hudY += 15;

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
    
    sf::Text powerText("Space - Use Power-up", font, 14);
    powerText.setFillColor(sf::Color::White);
    powerText.setPosition(hudX, hudY);
    window.draw(powerText);
    hudY += lineSpacing;
    
    sf::Text escapeText("Escape - Pause", font, 14);
    escapeText.setFillColor(sf::Color::White);
    escapeText.setPosition(hudX, hudY);
    window.draw(escapeText);
}


void GameScreen::drawControlInstructions(sf::RenderWindow& window) {
    int instructionsY = CONTROLS_Y;
    
    sf::Text instructTitle("CONTROLS", font, 16);
    instructTitle.setFillColor(sf::Color::Yellow);
    instructTitle.setPosition(GAME_START_X, instructionsY);
    window.draw(instructTitle);
    
    instructionsY += 25;
    
    sf::Text moveText("Arrow Keys - Move", font, 14);
    moveText.setFillColor(sf::Color::White);
    moveText.setPosition(GAME_START_X, instructionsY);
    window.draw(moveText);
    
    instructionsY += 20;
    
    sf::Text powerText("Space - Use Power-up", font, 14);
    powerText.setFillColor(sf::Color::White);
    powerText.setPosition(GAME_START_X, instructionsY);
    window.draw(powerText);
    
    instructionsY += 20;
    
    sf::Text escapeText("Escape - Return to Menu", font, 14);
    escapeText.setFillColor(sf::Color::White);
    escapeText.setPosition(GAME_START_X, instructionsY);
    window.draw(escapeText);
}

void GameScreen::drawTimer(sf::RenderWindow& window) {
    int totalSeconds = static_cast<int>(gameTimer);
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

void GameScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    window.draw(backgroundSprite);
       
    drawGameArea(window);
    drawPlayerHUD(window);
    drawTimer(window);
    
    window.display();
}


void GameScreen::saveCurrentGame() {
    if (!currentPlayer || !saveGameManager) {
        cerr << "Cannot save: missing player or save manager" << endl;
        return;
    }
    
    SaveGame save;
    string saveID = saveGameManager->generateSaveID(currentPlayer->getUsername());
    
    save.setSaveID(saveID);
    save.setTimestamp(time(nullptr));
    save.setUsername(currentPlayer->getUsername());
    save.setPlayerID(currentPlayer->getPlayerID());
    save.setGameTimer(gameTimer);
    save.setTotalPointsEarnedThisGame(totalPointsEarnedThisGame);
    save.setPlayerPosition(playerX, playerY);
    save.setGrid(grid);
    save.setCurrentPlayerTotalPoints(currentPlayer->getTotalPoints());
    save.setBonusMultiplierCount(currentPlayer->getBonusMultiplierCount());
    save.setPowerUpCount(currentPlayer->getPowerUps());
    save.setPowerUpActive(currentPlayer->isPowerUpActive());
    save.setPowerUpTimeRemaining(currentPlayer->getPowerUpTimeRemaining());
    save.setEnemyCount(enemyCount);
    
    for (int i = 0; i < enemyCount; i++) {
        save.setEnemy(i, enemies[i].x, enemies[i].y, enemies[i].dx, enemies[i].dy);
    }
    
    int minSpd, maxSpd;
    minSpd = minSpeed;
    maxSpd = maxSpeed;
    save.setDifficulty(minSpd, maxSpd);
    
    if (saveGameManager->saveGame(save)) {
        cout << "Game saved with ID: " << saveID << endl;
    }
}

void GameScreen::loadGameFromSave(const SaveGame& save) {
    if (!currentPlayer) {
        cerr << "Cannot load: no player" << endl;
        return;
    }
    
    gameTimer = save.getGameTimer();
    totalPointsEarnedThisGame = save.getTotalPointsEarnedThisGame();
    
    save.getPlayerPosition(playerX, playerY);
    save.getGrid(grid);
    
    
    isLoadedGame = true;
    loadedSaveID = save.getSaveID();
    pointsBeforeLoad = currentPlayer->getTotalPoints();
    
    
    
    
    
    currentPlayer->setBonusMultiplierCount(save.getBonusMultiplierCount());
    
    
    enemyCount = save.getEnemyCount();
    for (int i = 0; i < enemyCount; i++) {
        SavedEnemy enemy = save.getEnemy(i);
        enemies[i].x = enemy.x;
        enemies[i].y = enemy.y;
        enemies[i].dx = enemy.dx;
        enemies[i].dy = enemy.dy;
    }
    
    int minSpd, maxSpd;
    save.getDifficulty(minSpd, maxSpd);
    minSpeed = minSpd;
    maxSpeed = maxSpd;
    
    gameRunning = true;
    gamePaused = false;
    clock.restart();
 
}

void GameScreen::run(sf::RenderWindow& window, bool& backToMenu, PauseScreen* pauseScreen, EndGameScreen* endScreen) {
    if (!isLoadedGame) {
        resetGame();
    } 
    else {
        gameRunning = true;
        gamePaused = false;
        timer = 0;
        clock.restart();
    }

    while (window.isOpen() && !backToMenu && gameRunning) {
        handleInput(window, backToMenu);
         
        if (gamePaused) {
            int pauseChoice = -1;
            
            
            if (isLoadedGame) {
                pauseScreen->disableSaveOption();  
            }
            else {
                pauseScreen->enableSaveOption();   
            }
            
            pauseScreen->run(window, pauseChoice);
            clock.restart();
            
            if (pauseChoice == 0) {  
                gamePaused = false;
            } 
            else if (pauseChoice == 1) {  
             
                if (!isLoadedGame) {
                    saveCurrentGame();
                    backToMenu = true;  
                    break;
                }
            } 
            else if (pauseChoice == 2) {  
                if (isLoadedGame) {
                    currentPlayer->setTotalPoints(pointsBeforeLoad);
                }
                backToMenu = true;
                leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
                leaderboard->saveToFile("leaderboard.txt");
                playerManager->saveToFile("players_test.txt");
                break;
            }
        }
        
        update();
        draw(window);
    }
    
    if (!gameRunning && !backToMenu && window.isOpen()) {
        int endChoice = -1;
        bool newHighScore = false;
        
        int pointsEarnedThisSession = totalPointsEarnedThisGame;
        
        currentPlayer->addPoints(pointsEarnedThisSession);
        leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
        
        if (pointsEarnedThisSession > currentPlayer->getHighScore()) {
            newHighScore = true;
            currentPlayer->setHighScore(pointsEarnedThisSession);
        }
        
        
        if (isLoadedGame && saveGameManager->deleteSave(loadedSaveID)) {

        }
        else {
            
            if (pointsEarnedThisSession > currentPlayer->getHighScore()) {
                newHighScore = true;
                currentPlayer->setHighScore(pointsEarnedThisSession);
            }
        }
        
        leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
        leaderboard->saveToFile("leaderboard.txt");
        playerManager->saveToFile("players_test.txt");
        
        endScreen->setFinalScore(pointsEarnedThisSession);
        endScreen->run(window, endChoice, newHighScore);
        
        if (endChoice == 0) {  
            resetGame();
            gameRunning = true;
            gamePaused = false;
            isLoadedGame = false;  
            totalPointsEarnedThisGame = 0;
            gameTimer = 0.0f;
            run(window, backToMenu, pauseScreen, endScreen);  
        } 
        else if (endChoice == 1) {  
            backToMenu = true;
            leaderboard->update(currentPlayer->getUsername(), currentPlayer->getTotalPoints());
            leaderboard->saveToFile("leaderboard.txt");
            playerManager->saveToFile("players_test.txt");
        }
    }
}