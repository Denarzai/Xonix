#include "../../include/screens/gameRoomScreen.h"
#include <iostream>
#include <string>

GameRoomScreen::GameRoomScreen(PlayerManager* pm, Leaderboard* lb, Player* cp, int w, int h)
    : playerManager(pm), leaderboard(lb), currentPlayer(cp), windowWidth(w), windowHeight(h),
      playerAddedToQueue(false), queueCheckTimer(0.0f), 
      matchedPlayer1(nullptr), matchedPlayer2(nullptr), matchFound(false), queueInitialized(false) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    
}

GameRoomScreen::~GameRoomScreen() {}

void GameRoomScreen::handleInput(sf::RenderWindow& window, bool& backToMenu) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            backToMenu = true;
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                backToMenu = true;
            }
        }
    }
}

void GameRoomScreen::update() {
    queueCheckTimer += updateClock.getElapsedTime().asSeconds();
    updateClock.restart();
}

void GameRoomScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    drawWaitingMessage(window);
    drawQueueList(window);
    
    sf::Text instructions("Press ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
    
    window.display();
}

void GameRoomScreen::drawWaitingMessage(sf::RenderWindow& window) {
    sf::Text titleText("MATCHMAKING QUEUE", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);

    if (playerAddedToQueue) {
        sf::Text searchingText("Searching for Opponent...", font, 28);
        searchingText.setFillColor(sf::Color::White);
        searchingText.setPosition((windowWidth - searchingText.getLocalBounds().width) / 2.0f, 100);
        window.draw(searchingText);

        int elapsed = static_cast<int>(queueCheckTimer);
        std::string timerStr = "Elapsed: " + std::to_string(elapsed) + "s";

        sf::Color timerColor = sf::Color::Red;
        if (queueCheckTimer < 5.0f && matchmaking.canMatchPlayers())
            timerColor = sf::Color::Green;

        sf::Text timerText(timerStr, font, 26);
        timerText.setFillColor(timerColor);
        timerText.setPosition((windowWidth - timerText.getLocalBounds().width) / 2.0f, 145);
        window.draw(timerText);
    }
}

void GameRoomScreen::drawQueueList(sf::RenderWindow& window) {
    sf::Text queueHeaderText("Queue Position (Sorted by Score)", font, 18);
    queueHeaderText.setFillColor(sf::Color::White);
    queueHeaderText.setPosition(100, 190);
    window.draw(queueHeaderText);
    
    int yPos = 230;
    int position = 1;
    int queueSize = matchmaking.getQueueSize();
    
    for (int i = 0; i < queueSize; i++) {
        Player* player = matchmaking.getPlayerAt(i);
        if (!player) continue;
        
        std::string queueEntry = std::to_string(position) + ". " + player->getUsername() 
                                + " - Score: " + std::to_string(player->getTotalPoints());
        
        sf::Text playerText(queueEntry, font, 16);
        playerText.setFillColor(player == currentPlayer ? sf::Color::Green : sf::Color::White);
        playerText.setPosition(120, yPos);
        window.draw(playerText);
        
        yPos += 40;
        position++;
    }
}

void GameRoomScreen::getMatchedPlayers(Player*& p1, Player*& p2) const {
    p1 = matchedPlayer1;
    p2 = matchedPlayer2;
}

bool GameRoomScreen::arePlayersMatched() const {
    return matchFound;
}

void GameRoomScreen::run(sf::RenderWindow& window, bool& backToMenu, 
                        Player*& matchedP1, Player*& matchedP2, bool& matchFoundOut) {
    
    matchFoundOut = false;
    queueCheckTimer = 0.0f;
    
    
    
    if (!queueInitialized && matchmaking.getQueueSize() == 0) {
        
        if (!playerAddedToQueue && currentPlayer) {
            matchmaking.addPlayerToQueue(currentPlayer);
            playerAddedToQueue = true;
        }
        
        
        if (playerManager && leaderboard) {
            LBNode leaderboardEnties[10];
            int playerCount = leaderboard->getSortedAscending(leaderboardEnties);
            for (int i = 0; i < playerCount; i++) {
                Player* p = playerManager->getPlayerByUsername(leaderboardEnties[i].name);
                if (p && p != currentPlayer) {
                    matchmaking.addPlayerToQueue(p);
                }
            }
        }
        queueInitialized = true;
    }
    
    updateClock.restart();
    
    while (window.isOpen() && !backToMenu && !matchFoundOut) {
        handleInput(window, backToMenu);
        update();
        draw(window);

        if (queueCheckTimer >= 5.0f) {
            if (matchmaking.canMatchPlayers()) {
                matchmaking.getMatchedPair(matchedPlayer1, matchedPlayer2);
                matchFoundOut = true;
                matchedP1 = matchedPlayer1;
                matchedP2 = matchedPlayer2;
                queueCheckTimer = 0.0f;
                break;
            }
            
        }
    }
}

void GameRoomScreen::dequeueMatchedPlayers() {
    if (matchedPlayer1) {
        matchmaking.removePlayerFromQueue();
    }
    if (matchedPlayer2) {
        matchmaking.removePlayerFromQueue();
    }
    //cout << "remaining size: " <<matchmaking.getQueueSize() << endl;
}