#include "../../include/screens/mainMenu.h"
#include <iostream>

MainMenu::MainMenu(PlayerManager& mgr, Auth& authSystem, int width, int height)
    : manager(mgr), auth(authSystem), windowWidth(width), windowHeight(height), selectedOption(0) {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font for MainMenu!" << std::endl;
    }
    
    if (!titleFont.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading title font for MainMenu!" << std::endl;
    }
    
    normalColor = sf::Color::White;
    selectedColor = sf::Color::Yellow;
    titleColor = sf::Color::Cyan;
    
    optionYStart = 225.0f;
    optionSpacing = 40.0f;
    
    currentPlayer = nullptr;
    initializeOptions();
}

MainMenu::~MainMenu() {
}

void MainMenu::initializeOptions() {
    options[0] = "Start Game";
    options[1] = "Leaderboard";
    options[2] = "Profile";
    options[3] = "Friends";
    options[4] = "Themes";
    options[5] = "Saved Games";
    options[6] = "Logout";
    options[7] = "Exit";
}

void MainMenu::handleInput(sf::RenderWindow& window, bool& shouldExit, int& nextState) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            shouldExit = true;
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedOption--;
                if (selectedOption < 0) {
                    selectedOption = MAX_OPTIONS - 1;
                }
            }
            
            if (event.key.code == sf::Keyboard::Down) {
                selectedOption++;
                if (selectedOption >= MAX_OPTIONS) {
                    selectedOption = 0;
                }
            }
            
            if (event.key.code == sf::Keyboard::Return) {
                
                switch (selectedOption) {
                    case 0:
                        nextState = 100;   
                        break;
                    case 1: 
                        nextState = 2; 
                        break;
                    case 2: 
                        nextState = 3; 
                        break;
                    case 3: 
                        nextState = 4; 
                        break;
                    case 4: 
                        nextState = 5; 
                        break;
                    case 5: 
                        nextState = 6; 
                        break;
                    case 6: 
                        nextState = 7; 
                        break;
                    case 7: 
                        shouldExit = true;
                        window.close();
                        break;
                }
            }
            
            if (event.key.code == sf::Keyboard::Escape) {
                shouldExit = true;
                window.close();
            }
        }
    }
}

void MainMenu::update() {
    
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    drawTitle(window);
    drawOptions(window);
    //drawPlayerInfo(window);
    
    window.display();
}

void MainMenu::drawTitle(sf::RenderWindow& window) {
    sf::Text titleText("XONIX", titleFont, 100);
    titleText.setFillColor(titleColor);
    
    
    float titleX = (windowWidth - titleText.getLocalBounds().width) / 2.0f;
    titleText.setPosition(titleX, 60);
    
    window.draw(titleText);
    
    
    sf::Text subtitleText("Main Menu", font, 40);
    subtitleText.setFillColor(sf::Color::White);
    float subtitleX = (windowWidth - subtitleText.getLocalBounds().width) / 2.0f;
    subtitleText.setPosition(subtitleX, 165);
    
    window.draw(subtitleText);
}

void MainMenu::drawOptions(sf::RenderWindow& window) {
    float indicatorX = (windowWidth - 140) / 2.0f; 
    
    for (int i = 0; i < MAX_OPTIONS; i++) {
        sf::Text optionText(options[i], font, 24);
        
        if (i == selectedOption) {
            optionText.setFillColor(selectedColor);
            optionText.setStyle(sf::Text::Bold);
        } 
        else {
            optionText.setFillColor(normalColor);
        }
        float optionX = windowWidth / 2.0f - optionText.getLocalBounds().width / 2.0f;
        optionText.setPosition(optionX, optionYStart + (i * optionSpacing));
        window.draw(optionText);
    }
    
    
    sf::Text indicator(">", font, 24);
    indicator.setFillColor(selectedColor);
    indicator.setPosition(indicatorX - 40, optionYStart + (selectedOption * optionSpacing));
    window.draw(indicator);

    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void MainMenu::drawPlayerInfo(sf::RenderWindow& window) {
    if (currentPlayer) {
        sf::Text playerInfo("Player: " + currentPlayer->getUsername(), font, 18);
        playerInfo.setFillColor(sf::Color::White);
        playerInfo.setPosition(10, windowHeight - 30);
        window.draw(playerInfo);
        
        sf::Text scoreInfo("Score: " + std::to_string(currentPlayer->getTotalPoints()), font, 18);
        scoreInfo.setFillColor(sf::Color::Green);
        scoreInfo.setPosition(10, windowHeight - 50);
        window.draw(scoreInfo);
    }
}

int MainMenu::getSelectedOption() const {
    return selectedOption;
}

int MainMenu::getSelectedOptionId() const {
    switch (selectedOption) {
        case 0: return 1;  
        case 1: return 2;  
        case 2: return 3;  
        case 3: return 4;  
        case 4: return 5;  
        case 5: return 6;  
        case 6: return 7;  
        case 7: return 8;  
        default: return 0;
    }
}

void MainMenu::resetMenu() {
    selectedOption = 0;
}