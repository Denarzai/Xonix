#include "../../include/screens/endGameScreen.h"
#include <iostream>

EndGameScreen::EndGameScreen(int width, int height, const std::string& name)
    : windowWidth(width), windowHeight(height), selectedOption(0), 
      finalScore(0), playerName(name) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    
    if (!gameoverTexture.loadFromFile("images/gameover.png")) {
        std::cerr << "Warning: Could not load gameover.png" << std::endl;
    }
}

EndGameScreen::~EndGameScreen() {
}

void EndGameScreen::resetSelection() {
    selectedOption = 0;
}

void EndGameScreen::handleInput(sf::RenderWindow& window, int& choice, bool& backToEnd) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            backToEnd = false;
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedOption = (selectedOption - 1 + 2) % 2;
            }
            if (event.key.code == sf::Keyboard::Down) {
                selectedOption = (selectedOption + 1) % 2;
            }
            
            if (event.key.code == sf::Keyboard::Return) {
                choice = selectedOption;
                backToEnd = false;
            }
        }
    }
}

void EndGameScreen::draw(sf::RenderWindow& window, bool newHighscore) {
    
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    
    sf::Text gameOverTitle("GAME OVER", font, 80);
    gameOverTitle.setFillColor(sf::Color::Red);
    gameOverTitle.setStyle(sf::Text::Bold);
    float titleWidth = gameOverTitle.getLocalBounds().width;
    gameOverTitle.setPosition(windowWidth / 2 - titleWidth / 2, 50);
    window.draw(gameOverTitle);
    
    
    sf::Text playerText("Player: " + playerName, font, 28);
    playerText.setFillColor(sf::Color::White);
    float playerWidth = playerText.getLocalBounds().width;
    playerText.setPosition(windowWidth / 2 - playerWidth / 2, 160);
    window.draw(playerText);

    if(newHighscore){
        sf::Text highScore("New High Score", font, 28);
        highScore.setFillColor(sf::Color::Red);
        float playerWidth = highScore.getLocalBounds().width;
        highScore.setPosition(windowWidth / 2 - playerWidth / 2, 210);
        window.draw(highScore);

        
        sf::Text scoreText("Final Score: " + std::to_string(finalScore), font, 48);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setStyle(sf::Text::Bold);
        float scoreWidth = scoreText.getLocalBounds().width;
        scoreText.setPosition(windowWidth / 2 - scoreWidth / 2, 260);
        window.draw(scoreText);
    }
    else{
        
        sf::Text scoreText("Final Score: " + std::to_string(finalScore), font, 48);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setStyle(sf::Text::Bold);
        float scoreWidth = scoreText.getLocalBounds().width;
        scoreText.setPosition(windowWidth / 2 - scoreWidth / 2, 240);
        window.draw(scoreText);
    }
    
    
    std::string options[] = {"RESTART GAME", "RETURN TO MENU"};
    int optionY = 360;
    
    for (int i = 0; i < 2; i++) {
        sf::Text optionText(options[i], font, 40);
        
        if (i == selectedOption) {
            optionText.setFillColor(sf::Color::Yellow);
            optionText.setStyle(sf::Text::Bold);
            
            
            sf::Text selector("> ", font, 40);
            selector.setFillColor(sf::Color::Yellow);
            selector.setPosition(windowWidth / 2 - 350, optionY);
            window.draw(selector);
        } else {
            optionText.setFillColor(sf::Color::White);
        }
        
        float textWidth = optionText.getLocalBounds().width;
        optionText.setPosition(windowWidth / 2 - textWidth / 2, optionY);
        window.draw(optionText);
        
        optionY += 80;
    }
    
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void EndGameScreen::run(sf::RenderWindow& window, int& choice, bool newHighScore) {
    bool backToEnd = true;
    
    while (window.isOpen() && backToEnd) {
        handleInput(window, choice, backToEnd);
        draw(window, newHighScore);
        window.display();
    }
}