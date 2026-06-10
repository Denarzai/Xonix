#pragma once
using namespace std;
#include <SFML/Graphics.hpp>

class Button {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Text text;
    bool hovered;

public:
    Button(float x, float y, float w, float h, const string& str, sf::Font& font);

    void draw(sf::RenderWindow& window);
    bool isHovered(const sf::Vector2f& mousePos);
    bool isClicked(sf::RenderWindow& window);
    void setPosition(float x, float y);
};
