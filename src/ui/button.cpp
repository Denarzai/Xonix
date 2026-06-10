#include "../../include/ui/button.h"

Button::Button(float x, float y, float w, float h, const string& str, sf::Font& font) {
    texture.loadFromFile("../images/buttonBar.png");

    sprite.setTexture(texture);
    sprite.setPosition(x - 5, y - 5);
    sprite.setScale(260.0f / texture.getSize().x, 60.0f / texture.getSize().y);

    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(22);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(x + (w - textBounds.width) / 2 - textBounds.left, y + (h - textBounds.height) / 2 - textBounds.top);

    text.setFillColor(sf::Color::White);

    hovered = false;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(text);
}

bool Button::isHovered(const sf::Vector2f& mousePos) {
    hovered = sprite.getGlobalBounds().contains(mousePos);    
    return hovered;
}

bool Button::isClicked(sf::RenderWindow& window) {
    return hovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Button::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    text.setPosition(x - 5, y - 5);
}