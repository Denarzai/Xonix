#include "../../include/ui/textBox.h"

TextBox::TextBox(float x, float y, float w, float h, sf::Font& font) {
    box.setPosition(x, y);
    box.setSize({w, h});
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 5, y + 5);

    selected = false;
}

void TextBox::handleInput(sf::Event event) {
    if (!selected) return;

    if (event.type == sf::Event::TextEntered) {
        char c = event.text.unicode;
        if (c == 8 && !text.getString().isEmpty()) {  
            string s = text.getString();
            s.pop_back();
            text.setString(s);
        }
        else if (c >= 32 && c <= 126) {
            string s = text.getString();
            s.push_back(c);
            text.setString(s);
        }
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

string TextBox::getText() const {
    return text.getString();
}

void TextBox::setText(string s)  {
    text.setString(s);
}

void TextBox::setSelected(bool sel) {
    selected = sel;
    box.setOutlineColor(sel ? sf::Color::Yellow : sf::Color::Black);
}

bool TextBox::isHovered(const sf::Vector2f& mousePos) const {
    return box.getGlobalBounds().contains(mousePos);
}

sf::Vector2f TextBox::getPosition() const {
    return box.getPosition();
}

void TextBox::setPosition(float x, float y) {
    box.setPosition(x, y);
    text.setPosition(x + 5, y + 5);
}