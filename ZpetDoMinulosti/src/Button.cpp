#include "Button.h"
#include <cmath> /

Button::Button(float x, float y, float width, float height, const sf::String& text, const sf::Font& font)
    : buttonText(font) 
{
    shape.setPosition({x, y});
    shape.setSize({width, height});
    shape.setFillColor(sf::Color(0, 50, 100)); 
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(0, 255, 255));   

    buttonText.setString(text);                
    buttonText.setCharacterSize(22);
    buttonText.setFillColor(sf::Color::White);

    setText(text);
}

void Button::draw(sf::RenderWindow& window, bool censored)
{
    window.draw(shape);

    bool isHovered = false;
    
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);

    if (shape.getGlobalBounds().contains(mousePos)) {
        isHovered = true;
    }

    if (censored && !isHovered)
    {
        sf::RectangleShape mask;
        sf::Vector2f size = shape.getSize();
        
        mask.setSize({size.x - 10.0f, size.y - 10.0f});
        mask.setOrigin({mask.getSize().x / 2.0f, mask.getSize().y / 2.0f});
        mask.setFillColor(sf::Color(10, 10, 10)); 
        
        sf::Vector2f pos = shape.getPosition();
        mask.setPosition({pos.x + size.x / 2.0f, pos.y + size.y / 2.0f});
        
        window.draw(mask);
    }
    else
    {
        window.draw(buttonText);
    }
}

bool Button::isClicked(sf::Vector2f mousePos)
{
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setText(const sf::String& text)
{
    buttonText.setCharacterSize(22);
    buttonText.setString(text);

    float containerWidth = shape.getSize().x - 20.0f; 
    
    std::wstring wrappedText = text;
    std::wstring currentLine;
    std::wstring finalString;
    
    std::size_t startPos = 0;
    std::size_t spacePos = wrappedText.find(L' ');

    while (spacePos != std::wstring::npos || startPos < wrappedText.length())
    {
        std::wstring word;
        if (spacePos == std::wstring::npos) {
            word = wrappedText.substr(startPos);
            startPos = wrappedText.length();
        } else {
            word = wrappedText.substr(startPos, spacePos - startPos);
            startPos = spacePos + 1;
        }

        std::wstring testLine = currentLine.empty() ? word : currentLine + L" " + word;
        
        buttonText.setString(testLine);
        if (buttonText.getLocalBounds().size.x > containerWidth)
        {
            if (!finalString.empty()) finalString += L"\n";
            finalString += currentLine;
            currentLine = word;
        }
        else
        {
            currentLine = testLine;
        }

        spacePos = wrappedText.find(L' ', startPos);
    }
    if (!finalString.empty()) finalString += L"\n";
    finalString += currentLine;

    buttonText.setString(finalString);

    sf::FloatRect bounds = buttonText.getLocalBounds();
    
    buttonText.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });

    sf::Vector2f btnPos = shape.getPosition();
    sf::Vector2f btnSize = shape.getSize();

    buttonText.setPosition({
        btnPos.x + btnSize.x / 2.0f,
        btnPos.y + btnSize.y / 2.0f
    });
}

void Button::setPosition(float x, float y)
{
    shape.setPosition({x, y});   

    sf::Vector2f size = shape.getSize();
    buttonText.setPosition({
        x + size.x / 2.0f,
        y + size.y / 2.0f
    });
}

void Button::setPosition(sf::Vector2f pos)
{
    shape.setPosition(pos);

    sf::Vector2f size = shape.getSize();
    buttonText.setPosition
    ({
        pos.x + size.x / 2.0f,
        pos.y + size.y / 2.0f
    });
}

sf::Vector2f Button::getPosition() const
{
    return shape.getPosition();
}

void Button::moveText(float x, float y)
{
    buttonText.move({x, y});
}