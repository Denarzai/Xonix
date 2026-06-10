#include "../../include/screens/inventoryScreen.h"
#include <iostream>

InventoryScreen::InventoryScreen(ThemeInventory* inventory, PlayerManager* playerManager, float width, float height)
    : themeInventory(inventory), manager(playerManager), windowWidth(width), windowHeight(height) {}

void InventoryScreen::run(sf::RenderWindow& window, Player* currentPlayer, bool& backToMenu) {
    backToMenu = false;
    sf::Font font;
    font.loadFromFile("../data/Picopixel.ttf");
    
    int themeCount = 0;
    Theme* allThemes = themeInventory->getAllThemesInOrder(themeCount);

    sf::Texture* themePreviews = new sf::Texture[themeCount]; 
    for (int i = 0; i < themeCount; i++) {
        std::string path = "../images/preview" + std::to_string(i) + ".png";
        if (!themePreviews[i].loadFromFile(path)) {
            std::cerr << "Failed to load theme preview: " << path << std::endl;
        }
    }

    int selectedTheme = 0;
    bool selectingTheme = true;
    
    while (selectingTheme && window.isOpen()) {
    window.clear(sf::Color::Black);

    
    sf::Text titleText("SELECT THEME", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((window.getSize().x - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);

    sf::Vector2f containerSize(300, 100);
    float padding = 20;
    float totalHeight = themeCount * (containerSize.y + padding);
    float startY = (windowHeight - totalHeight) / 2.0f;

    for (int i = 0; i < themeCount; i++) {
        sf::Vector2f pos(100, startY + i * (containerSize.y + padding));

        sf::RectangleShape container(containerSize);
        container.setPosition(pos);
        container.setFillColor(sf::Color(30,30,30));
        container.setOutlineThickness(2);
        container.setOutlineColor(i == selectedTheme ? sf::Color::Yellow : sf::Color::White);
        window.draw(container);

        
        sf::Sprite previewSprite;
        previewSprite.setTexture(themePreviews[i]);
        float scaleX = (containerSize.x / 3) / themePreviews[i].getSize().x;  
        float scaleY = (containerSize.y - 20) / themePreviews[i].getSize().y;
        previewSprite.setScale(scaleX, scaleY);
        previewSprite.setPosition(pos.x + 10, pos.y + 10);
        window.draw(previewSprite);

        
        sf::Text themeText(allThemes[i].getThemeName(), font, 25);
        themeText.setFillColor(sf::Color::White);
        themeText.setPosition(pos.x + containerSize.x / 3 + 20, pos.y + containerSize.y / 2 - 15);
        window.draw(themeText);
    }
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(window.getSize().x / 2 - instrWidth / 2, window.getSize().y - 50);
    window.draw(instructions);

    window.display();
    
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Up) {
                selectedTheme = (selectedTheme - 1 + themeCount) % themeCount;
            } else if (e.key.code == sf::Keyboard::Down) {
                selectedTheme = (selectedTheme + 1) % themeCount;
            } else if (e.key.code == sf::Keyboard::Return) {
                themeInventory->setCurrentTheme(allThemes[selectedTheme].getThemeId());
                if(currentPlayer->getThemeID() != selectedTheme){
                    currentPlayer->setThemeID(selectedTheme);
                    manager->saveToFile("players_test.txt");
                }
                selectingTheme = false;
                backToMenu = true;
            } else if (e.key.code == sf::Keyboard::Escape) {
                selectingTheme = false;
                backToMenu = true;
            }
        }
    }
}

    
    if (allThemes != nullptr) {
        delete[] allThemes;
        allThemes = nullptr;
    }
}