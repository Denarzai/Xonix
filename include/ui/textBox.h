#pragma once
using namespace std;
#include <SFML/Graphics.hpp>

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    bool selected;

public:
    TextBox(float x, float y, float w, float h, sf::Font& font);

    void handleInput(sf::Event event);
    void draw(sf::RenderWindow& window);
    string getText() const;
    void setText(string) ;
    void setSelected(bool sel);
    bool isHovered(const sf::Vector2f& mousePos) const;
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    bool isEmpty() {
        return text.getString().isEmpty();
    }
    bool isSelected() { return selected; }
};
