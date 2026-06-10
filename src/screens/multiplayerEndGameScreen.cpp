#include "../../include/screens/multiplayerEndGameScreen.h"
#include <iostream>

MultiplayerEndGameScreen::MultiplayerEndGameScreen(int width, int height, const std::string& p1Name, const std::string& p2Name)
    : windowWidth(width), windowHeight(height), selectedOption(0), 
      player1Name(p1Name), player2Name(p2Name), isGameRoomMode(false) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    
    if (!gameoverTexture.loadFromFile("images/gameover.png")) {
        std::cerr << "Warning: Could not load gameover.png" << std::endl;
    }
}

MultiplayerEndGameScreen::~MultiplayerEndGameScreen() {
}

void MultiplayerEndGameScreen::setGameRoomMode(bool isGameRoom) {
    isGameRoomMode = isGameRoom;
}

void MultiplayerEndGameScreen::resetSelection() {
    selectedOption = 0;
}

void MultiplayerEndGameScreen::handleInput(sf::RenderWindow& window, int& choice, bool& backToEnd) {
    int maxOptions = isGameRoomMode ? 3 : 2;  
    
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            backToEnd = false;
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
            }
            if (event.key.code == sf::Keyboard::Down) {
                selectedOption = (selectedOption + 1) % maxOptions;
            }
            
            if (event.key.code == sf::Keyboard::Return) {
                choice = selectedOption;
                backToEnd = false;
            }
        }
    }
}

void MultiplayerEndGameScreen::draw(sf::RenderWindow& window) {
    
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    
    sf::Text gameOverTitle("GAME OVER", font, 80);
    gameOverTitle.setFillColor(sf::Color::Red);
    gameOverTitle.setStyle(sf::Text::Bold);
    float titleWidth = gameOverTitle.getLocalBounds().width;
    gameOverTitle.setPosition(windowWidth / 2 - titleWidth / 2, 30);
    window.draw(gameOverTitle);
    
    
    std::string winnerText;
    sf::Color winnerColor;
    if (winnerName.empty() || player1Score == player2Score) {
        winnerText = "IT'S A TIE!";
        winnerColor = sf::Color::Cyan;
    } 
    else {
        winnerText = winnerName + " WINS!";
        winnerColor = sf::Color::Green;
    }
    
    sf::Text winnerLabel(winnerText, font, 50);
    winnerLabel.setFillColor(winnerColor);
    winnerLabel.setStyle(sf::Text::Bold);
    float winnerWidth = winnerLabel.getLocalBounds().width;
    winnerLabel.setPosition(windowWidth / 2 - winnerWidth / 2, 120);
    window.draw(winnerLabel);
    
    
    sf::Text p1Text("Player 1: " + player1Name, font, 28);
    p1Text.setFillColor(sf::Color::White);
    float p1Width = p1Text.getLocalBounds().width;
    p1Text.setPosition(windowWidth / 2 - p1Width / 2 - 300, 210);
    window.draw(p1Text);
    
    sf::Text p1ScoreText("Score: " + std::to_string(player1Score), font, 36);
    p1ScoreText.setFillColor(sf::Color::Yellow);
    p1ScoreText.setStyle(sf::Text::Bold);
    float p1ScoreWidth = p1ScoreText.getLocalBounds().width;
    p1ScoreText.setPosition(windowWidth / 2 - p1ScoreWidth / 2 - 300, 245);
    window.draw(p1ScoreText);
    
    
    sf::Text p2Text("Player 2: " + player2Name, font, 28);
    p2Text.setFillColor(sf::Color::White);
    float p2Width = p2Text.getLocalBounds().width;
    p2Text.setPosition(windowWidth / 2 - p2Width / 2 + 300, 210);
    window.draw(p2Text);
    
    sf::Text p2ScoreText("Score: " + std::to_string(player2Score), font, 36);
    p2ScoreText.setFillColor(sf::Color::Yellow);
    p2ScoreText.setStyle(sf::Text::Bold);
    float p2ScoreWidth = p2ScoreText.getLocalBounds().width;
    p2ScoreText.setPosition(windowWidth / 2 - p2ScoreWidth / 2 + 300, 245);
    window.draw(p2ScoreText);
    
    
    std::string options[3];
    options[0] = "RESTART GAME";
    options[1] = "RETURN TO MENU";
    if (isGameRoomMode) {
        options[2] = "RETURN TO QUEUE";
    }
    
    int maxOptions = isGameRoomMode ? 3 : 2;
    int optionY = 340;
    
    for (int i = 0; i < maxOptions; i++) {
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

void MultiplayerEndGameScreen::run(sf::RenderWindow& window, int& choice, int p1Score, int p2Score) {
    player1Score = p1Score;
    player2Score = p2Score;
    if(player1Score > player2Score) winnerName = player1Name;
    else if (player1Score < player2Score) winnerName = player2Name;
    else winnerName = "";
    bool backToEnd = true;
    
    while (window.isOpen() && backToEnd) {
        handleInput(window, choice, backToEnd);
        draw(window);
        window.display();
    }
}