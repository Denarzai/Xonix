#include "../../include/screens/pauseScreen.h"
#include <iostream>

PauseScreen::PauseScreen(int width, int height)
    : windowWidth(width), windowHeight(height), selectedOption(0) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
}

PauseScreen::~PauseScreen() {
}

void PauseScreen::resetSelection() {
    selectedOption = 0;
}

void PauseScreen::handleInput(sf::RenderWindow& window, int& choice, bool& backToPause) {
    int maxOptions = 3;
    
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Up) {
                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
                if (!savOptionEnabled && selectedOption == 1) {
                    selectedOption = 0; 
                }
            }
            else if (e.key.code == sf::Keyboard::Down) {
                selectedOption = (selectedOption + 1) % maxOptions;
                if (!savOptionEnabled && selectedOption == 1) {
                    selectedOption = 2; 
                }
            }
            else if (e.key.code == sf::Keyboard::Return) {
                choice = selectedOption;
                backToPause = false;
            }
        }
    }
}

void PauseScreen::draw(sf::RenderWindow& window) {
    
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    
    sf::Text pauseTitle("GAME PAUSED", font, 60);
    pauseTitle.setFillColor(sf::Color::Cyan);
    float titleWidth = pauseTitle.getLocalBounds().width;
    pauseTitle.setPosition(windowWidth / 2 - titleWidth / 2, 80);
    window.draw(pauseTitle);
    
    
    std::string options[] = {"RESUME", "SAVE GAME", "RETURN TO MENU"};
    int optionY = 230;
    
    for (int i = 0; i < 3; i++) {
        if(!savOptionEnabled && i == 1) continue;
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







/*
void PauseScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    sf::Font font;
    font.loadFromFile("../data/Picopixel.ttf");
    
    sf::Text title("PAUSED", font, 50);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition(windowWidth / 2 - 100, 100);
    window.draw(title);
    
    
    sf::Text resumeText("Resume Game", font, 30);
    resumeText.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    resumeText.setPosition(windowWidth / 2 - 100, 200);
    window.draw(resumeText);
    
    
    int yOffset = 250;
    if (savOptionEnabled) {
        sf::Text saveText("Save Game", font, 30);
        saveText.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
        saveText.setPosition(windowWidth / 2 - 80, yOffset);
        window.draw(saveText);
        yOffset = 300;
    }
    
    
    sf::Text returnText("Return to Menu", font, 30);
    int returnChoice = savOptionEnabled ? 2 : 1;
    returnText.setFillColor(selectedOption == returnChoice ? sf::Color::Yellow : sf::Color::White);
    returnText.setPosition(windowWidth / 2 - 120, yOffset);
    window.draw(returnText);
    
    window.display();
} */

void PauseScreen::run(sf::RenderWindow& window, int& choice) {
    sf::View view = window.getDefaultView();
    window.setView(view);
    bool backToPause = true;
    
    while (window.isOpen() && backToPause) {
        handleInput(window, choice, backToPause);
        draw(window);
        window.display();
    }
}