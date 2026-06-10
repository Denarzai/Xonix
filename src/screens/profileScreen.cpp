#include "../../include/screens/profileScreen.h"
#include <iostream>

ProfileScreen::ProfileScreen(Player* player, int width, int height)
    : currentPlayer(player), windowWidth(width), windowHeight(height) {
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
}

ProfileScreen::~ProfileScreen() {
}

void ProfileScreen::handleInput(sf::RenderWindow& window, bool& backToMenu) {
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

void ProfileScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    // Title
    sf::Text titleText("PLAYER PROFILE", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);

    if (currentPlayer) {
        float startX = 100;
        float startY = 120;
        float lineSpacing = 50;

        // Username
        sf::Text usernameText("Username: " + currentPlayer->getUsername(), font, 24);
        usernameText.setFillColor(sf::Color::White);
        usernameText.setPosition(startX, startY);
        window.draw(usernameText);

        // Nickname
        sf::Text nicknameText("Nickname: " + currentPlayer->getNickname(), font, 24);
        nicknameText.setFillColor(sf::Color::White);
        nicknameText.setPosition(startX, startY + lineSpacing);
        window.draw(nicknameText);

        // Email (optional)
        std::string emailStr = currentPlayer->getEmail().empty() ? "Email: N/A" : "Email: " + currentPlayer->getEmail();
        sf::Text emailText(emailStr, font, 24);
        emailText.setFillColor(sf::Color::White);
        emailText.setPosition(startX, startY + 2 * lineSpacing);
        window.draw(emailText);

        // Total Points
        sf::Text totalPointsText("Total Points: " + std::to_string(currentPlayer->getTotalPoints()), font, 24);
        totalPointsText.setFillColor(sf::Color::Green);
        totalPointsText.setPosition(startX, startY + 3 * lineSpacing);
        window.draw(totalPointsText);

        // High Score
        sf::Text highScoreText("High Score: " + std::to_string(currentPlayer->getHighScore()), font, 24);
        highScoreText.setFillColor(sf::Color::Yellow);
        highScoreText.setPosition(startX, startY + 4 * lineSpacing);
        window.draw(highScoreText);

        // Current Theme
        sf::Text themeText("Current Theme ID: " + std::to_string(currentPlayer->getThemeID()), font, 24);
        themeText.setFillColor(sf::Color::Magenta);
        themeText.setPosition(startX, startY + 5 * lineSpacing);
        window.draw(themeText);

        // Friend Count
        sf::Text friendCountText("Friends: " + std::to_string(currentPlayer->getFriendCount()), font, 24);
        friendCountText.setFillColor(sf::Color::Cyan);
        friendCountText.setPosition(startX, startY + 6 * lineSpacing);
        window.draw(friendCountText);

        // Power-Up Count (optional)
        sf::Text powerUpText("Power-Ups: " + std::to_string(currentPlayer->getPowerUps()), font, 24);
        powerUpText.setFillColor(sf::Color::Cyan);
        powerUpText.setPosition(startX, startY + 7 * lineSpacing);
        window.draw(powerUpText);
    }

    // Instructions (unchanged)
    sf::Text instructions("Press ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);

    window.display();
}


void ProfileScreen::run(sf::RenderWindow& window, bool& backToMenu) {
    backToMenu = false;
    
    while (window.isOpen() && !backToMenu) {
        handleInput(window, backToMenu);
        draw(window);
    }
}