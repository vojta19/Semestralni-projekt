#include "Button.h"
#include <cmath> // Pro jistotu

Button::Button(float x, float y, float width, float height, const sf::String& text, const sf::Font& font)
    : buttonText(font) // Inicializace fontu
{
    // 1. Nastavení vzhledu obdélníku
    shape.setPosition({x, y});
    shape.setSize({width, height});
    shape.setFillColor(sf::Color(0, 50, 100)); 
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(0, 255, 255));   

    // 2. Nastavení textu
    buttonText.setString(text);                
    buttonText.setCharacterSize(22);
    buttonText.setFillColor(sf::Color::White);

    // 3. Prvotní vycentrování (voláme naši funkci setText, která to udělá i se zalamováním)
    setText(text);
}

void Button::draw(sf::RenderWindow& window, bool censored)
{
    window.draw(shape);

    // Zjistíme, jestli je myš nad tlačítkem (pro cenzuru)
    bool isHovered = false;
    
    // Získáme pozici myši v okně
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);

    // Kontrola kolize
    if (shape.getGlobalBounds().contains(mousePos)) {
        isHovered = true;
    }

    // Logika Cenzury: Pokud je zapnutá A myš NENÍ na tlačítku -> Kreslíme černý blok
    if (censored && !isHovered)
    {
        sf::RectangleShape mask;
        // SFML 3.0 používá .x .y přímo v Vector2f
        sf::Vector2f size = shape.getSize();
        
        mask.setSize({size.x - 10.0f, size.y - 10.0f});
        mask.setOrigin({mask.getSize().x / 2.0f, mask.getSize().y / 2.0f});
        mask.setFillColor(sf::Color(10, 10, 10)); // Téměř černá
        
        // Střed masky dáme na střed tlačítka
        sf::Vector2f pos = shape.getPosition();
        mask.setPosition({pos.x + size.x / 2.0f, pos.y + size.y / 2.0f});
        
        window.draw(mask);
    }
    else
    {
        // Jinak vidíme text
        window.draw(buttonText);
    }
}

bool Button::isClicked(sf::Vector2f mousePos)
{
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::setText(const sf::String& text)
{
    // 1. Reset nastavení
    buttonText.setCharacterSize(22);
    buttonText.setString(text);

    float containerWidth = shape.getSize().x - 20.0f; 
    
    // --- ALGORITMUS PRO ZALAMOVÁNÍ TEXTU ---
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
        // SFML 3.0: getLocalBounds().size.x
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

    // --- CENTROVÁNÍ TEXTU ---
    sf::FloatRect bounds = buttonText.getLocalBounds();
    
    // SFML 3.0: position.x, size.x
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

// Základní nastavení pozice (float, float)
void Button::setPosition(float x, float y)
{
    shape.setPosition({x, y});   

    sf::Vector2f size = shape.getSize();
    buttonText.setPosition({
        x + size.x / 2.0f,
        y + size.y / 2.0f
    });
}

// --- NOVÉ METODY PRO CHAOS MÓD ---

// 1. Nastavení pozice pomocí Vector2f (pro std::shuffle v GamePlayScreen)
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

// 2. Získání pozice (abychom věděli, kde tlačítko je před mícháním)
sf::Vector2f Button::getPosition() const
{
    return shape.getPosition();
}

void Button::moveText(float x, float y)
{
    buttonText.move({x, y});
}