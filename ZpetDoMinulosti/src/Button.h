#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(float x, float y, float width, float height, const std::string&text, const sf::Font&font); //konstruktor, rika kde tlacitko bude a co v nem bude

    void draw(sf::RenderWindow&window); //vykresleni tlacitka do okna

    void setText(const std::string&text);

    bool isClicked(sf::Vector2f mousePos); //zjisteni, zda myš klikla uvnitr tlacitka

private:
    sf::RectangleShape shape; //samotny obdelnik
    sf::Text buttonText;      //napis

};