#include "../../include/screens/leaderboardScreen.h"
#include <iostream>

LeaderboardScreen::LeaderboardScreen(PlayerManager& mgr, Leaderboard& lb,int width, int height)
    : manager(mgr), leaderboard(lb), windowWidth(width), windowHeight(height) {
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
}

LeaderboardScreen::~LeaderboardScreen() {
}

void LeaderboardScreen::handleInput(sf::RenderWindow& window, bool& backToMenu) {
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

void LeaderboardScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    
    sf::Text titleText("LEADERBOARD", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);

    
    sf::Text infoText("Top 10 Players", font, 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(100, 100);
    window.draw(infoText);

    
    sf::RectangleShape separator(sf::Vector2f(windowWidth - 200, 2));
    separator.setFillColor(sf::Color::Cyan);
    separator.setPosition(100, 130);
    window.draw(separator);

    
    LBNode sorted[10];
    int n = leaderboard.getSortedDescending(sorted); 

    
    float rankX = 120;
    float nameX = 160;
    float scoreX = 400;

    for (int i = 0; i < n; i++) {
        
        sf::RectangleShape bg(sf::Vector2f(400, 28));
        bg.setPosition(110, 140 + i * 30);
        bg.setFillColor((i % 2 == 0) ? sf::Color(50,50,50,100) : sf::Color(70,70,70,100));
        window.draw(bg);

        
        Player* p = manager.getPlayerByUsername(sorted[i].name);
        std::string nameStr = p ? p->getUsername() : "Unknown";

        
        sf::Text rankText(std::to_string(i+1) + ".", font, 20);
        rankText.setFillColor(sf::Color::White);
        rankText.setPosition(rankX, 140 + i * 30);
        window.draw(rankText);

        
        sf::Text nameText(nameStr, font, 20);
        nameText.setFillColor(nameStr == currentPlayer->getUsername() ? sf::Color::Yellow : sf::Color::White);
        nameText.setPosition(nameX, 140 + i * 30);
        window.draw(nameText);

        
        sf::Text scoreText(std::to_string(sorted[i].score), font, 20);
        scoreText.setFillColor(nameStr == currentPlayer->getUsername() ? sf::Color::Yellow : sf::Color::White);
        scoreText.setPosition(scoreX, 140 + i * 30);
        window.draw(scoreText);
    }

    
    sf::Text instructions("Press ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);

    window.display();
}



void LeaderboardScreen::run(sf::RenderWindow& window, Player* pl, bool& backToMenu) {
    backToMenu = false;
    currentPlayer = pl;
    
    while (window.isOpen() && !backToMenu) {
        handleInput(window, backToMenu);
        draw(window);
    }
}