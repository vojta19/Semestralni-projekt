#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string&text,const sf::Font&font)
:buttonText(font)
{
    //nastaveni vzhledu obdelniku
    shape.setPosition({x,y});
    shape.setSize({width,height});
    shape.setFillColor(sf::Color(70,70,70)); //tmave seda barva
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);    //bily ramecek
    
    //nastaveni textu
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setFillColor(sf::Color::White);

    //zjistime rozmery textu
    sf::FloatRect textBounds=buttonText.getLocalBounds();

    
    //nastavime stred textu presne do jeho geometrickeho stredu
    buttonText.setOrigin
    (
        {
            textBounds.position.x+textBounds.size.x/2.0f,
            textBounds.position.y+textBounds.size.y/2.0f
        }
    );

    
    //a tento stred presunume do stredu tlacitka
    buttonText.setPosition
    (
        {
            x+width/2.0f,
            y+height/2.0f
        }
    );
}

void Button::draw(sf::RenderWindow&window)
{
    window.draw(shape);
    window.draw(buttonText);
}

bool Button::isClicked(sf::Vector2f mousePos)
{
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setText(const std::string&text)
{
    buttonText.setString(text);

    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin
    (
        {
            textBounds.position.x+textBounds.size.x/2.0f,
            textBounds.position.y+textBounds.size.y/2.0f
        }
    );
}

void Button::setPosition(float x, float y)
{
    shape.setPosition({x,y});   //posunume obdelnik

    //posuneme text a musime ho dat doprostred obdelniku
    sf::Vector2f size=shape.getSize();
    buttonText.setPosition
    (
        {
            x+size.x/2.0f,
            y+size.y/2.0f
        }
    );
}