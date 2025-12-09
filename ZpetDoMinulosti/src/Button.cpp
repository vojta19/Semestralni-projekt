#include "Button.h"

Button::Button(float x, float y, float width, float height, const sf::String& text, const sf::Font& font)
    : buttonText(font)
{
    // 1. Nastavení vzhledu obdélníku
    shape.setPosition({x, y});
    shape.setSize({width, height});
    shape.setFillColor(sf::Color(0, 50, 100)); 
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(0,255,255));   

    // 2. Nastavení textu
    buttonText.setString(text);                
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);

    // 3. Zjistíme rozměry textu pro centrování
    sf::FloatRect textBounds = buttonText.getLocalBounds();

    // Nastavíme střed textu přesně do jeho geometrického středu
    buttonText.setOrigin
    ({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    // A tento střed přesuneme do středu tlačítka
    buttonText.setPosition
    ({
        x + width / 2.0f,
        y + height / 2.0f
    });
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(buttonText);
}

bool Button::isClicked(sf::Vector2f mousePos)
{
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setText(const sf::String& text)
{
    buttonText.setString(text);

    // Musíme znovu přepočítat střed, protože nový text má jinou délku
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    
    buttonText.setOrigin
    ({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });
    
    // Pro jistotu ujistíme, že je text stále na středu tlačítka
    sf::Vector2f btnPos = shape.getPosition();
    sf::Vector2f btnSize = shape.getSize();
    
    buttonText.setPosition
    ({
        btnPos.x + btnSize.x / 2.0f,
        btnPos.y + btnSize.y / 2.0f
    });
}

void Button::setPosition(float x, float y)
{
    shape.setPosition({x, y});   // Posuneme obdélník

    // Posuneme text a musíme ho dát doprostřed obdélníku
    sf::Vector2f size = shape.getSize();
    buttonText.setPosition
    ({
        x + size.x / 2.0f,
        y + size.y / 2.0f
    });
}