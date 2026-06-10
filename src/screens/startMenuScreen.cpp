#include "../../include/screens/startMenuScreen.h"
#include <iostream>

StartMenuScreen::StartMenuScreen(int width, int height)
    : windowWidth(width), windowHeight(height), selectedOption(0)
{
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font for StartMenu!" << std::endl;
    }

    if (!titleFont.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading title font!" << std::endl;
    }

    normalColor = sf::Color::White;
    selectedColor = sf::Color::Yellow;

    optionYStart = 225.f;
    optionSpacing = 40.f;

    options[0] = "Easy";
    options[1] = "Medium";
    options[2] = "Hard";
    options[3] = "Multiplayer";
    options[4] = "Game Room";
}

void StartMenuScreen::run(sf::RenderWindow& window, int& choice, bool& backToMenu)
{
    choice = 0;
    backToMenu = false;
    sf::Event e;

    while (window.isOpen()) {
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Up) {
                    selectedOption--;
                    if (selectedOption < 0) selectedOption = OPTION_COUNT - 1;
                }

                if (e.key.code == sf::Keyboard::Down) {
                    selectedOption++;
                    if (selectedOption >= OPTION_COUNT) selectedOption = 0;
                }

                if (e.key.code == sf::Keyboard::Return) {
                    choice = selectedOption + 1;  
                    return;
                }

                if (e.key.code == sf::Keyboard::Escape) {
                    backToMenu = true;
                    return;
                }
            }
        }

        draw(window);
    }
}

void StartMenuScreen::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);

    drawTitle(window);
    drawOptions(window);

    window.display();
}

void StartMenuScreen::drawTitle(sf::RenderWindow& window)
{
    sf::Text title("Start Game", titleFont, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition((windowWidth - title.getLocalBounds().width) / 2.f, 165);
    window.draw(title);
}

void StartMenuScreen::drawOptions(sf::RenderWindow& window)
{
    float indicatorX = (windowWidth - 140) / 2.f;

    for (int i = 0; i < OPTION_COUNT; i++) {
        sf::Text text(options[i], font, 24);
        text.setFillColor(i == selectedOption ? selectedColor : normalColor);
        text.setStyle(i == selectedOption ? sf::Text::Bold : sf::Text::Regular);
        float optionX = windowWidth / 2.0f - text.getLocalBounds().width / 2.0f;
        text.setPosition(optionX, optionYStart + i * optionSpacing);
        window.draw(text);
    }

    sf::Text arrow(">", font, 24);
    arrow.setFillColor(selectedColor);
    arrow.setPosition(indicatorX - 40, optionYStart + selectedOption * optionSpacing);
    window.draw(arrow);

    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}
