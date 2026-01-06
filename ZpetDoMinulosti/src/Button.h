#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(float x, float y, float width, float height, const sf::String& text, const sf::Font& font);

    
    void draw(sf::RenderWindow& window, bool censored = false);
    
    void setText(const sf::String& text);

    void setPosition(float x, float y);

    void setPosition(sf::Vector2f pos);

    sf::Vector2f getPosition() const;

    bool isClicked(sf::Vector2f mousePos);

    void moveText(float x,float y);

    void setBackgroundColor(sf::Color color);

private:
    sf::RectangleShape shape; // Samotný obdélník tlačítka
    sf::Text buttonText;      // Text uvnitř tlačítka
};