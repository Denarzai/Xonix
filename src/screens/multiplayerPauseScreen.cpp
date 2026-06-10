#include "../../include/screens/multiplayerPauseScreen.h"
#include <iostream>

MultiplayerPauseScreen::MultiplayerPauseScreen(int width, int height)
    : windowWidth(width), windowHeight(height), selectedOption(0) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
}

MultiplayerPauseScreen::~MultiplayerPauseScreen() {
}

void MultiplayerPauseScreen::resetSelection() {
    selectedOption = 0;
}

void MultiplayerPauseScreen::handleInput(sf::RenderWindow& window, int& choice, bool& backToPause) {
    int maxOptions = 2;
    
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Up) {
                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
            }
            else if (e.key.code == sf::Keyboard::Down) {
                selectedOption = (selectedOption + 1) % maxOptions;
            }
            else if (e.key.code == sf::Keyboard::Return) {
                choice = selectedOption;
                backToPause = false;
            }
        }
    }
}

void MultiplayerPauseScreen::draw(sf::RenderWindow& window) {
    
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    
    sf::Text pauseTitle("GAME PAUSED", font, 60);
    pauseTitle.setFillColor(sf::Color::Cyan);
    float titleWidth = pauseTitle.getLocalBounds().width;
    pauseTitle.setPosition(windowWidth / 2 - titleWidth / 2, 80);
    window.draw(pauseTitle);
    
    
    std::string options[] = {"RESUME", "RETURN TO MENU"};
    int optionY = 230;
    
    for (int i = 0; i < 2; i++) {
        sf::Text optionText(options[i], font, 40);
        
        if (i == selectedOption) {
            optionText.setFillColor(sf::Color::Yellow);
            optionText.setStyle(sf::Text::Bold);
            
            
            sf::Text selector("> ", font, 40);
            selector.setFillColor(sf::Color::Yellow);
            selector.setPosition(windowWidth / 2 - 300, optionY);
            window.draw(selector);
        } else {
            optionText.setFillColor(sf::Color::White);
        }
        
        float textWidth = optionText.getLocalBounds().width;
        optionText.setPosition(windowWidth / 2 - textWidth / 2, optionY);
        window.draw(optionText);
        
        optionY += 80;
    }
    
    
    sf::Text instructions("Press ENTER to select or ESC to resume", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 60);
    window.draw(instructions);
}

void MultiplayerPauseScreen::run(sf::RenderWindow& window, int& choice) {
    sf::View view = window.getDefaultView();
    window.setView(view);
    bool backToPause = true;
    
    while (window.isOpen() && backToPause) {
        handleInput(window, choice, backToPause);
        draw(window);
        window.display();
    }
}