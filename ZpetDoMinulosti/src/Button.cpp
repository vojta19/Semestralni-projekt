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
    // 1. Resetujeme nastavení (velikost písma vrátíme na default, pokud by byla zmenšená)
    unsigned int characterSize = 22; // Výchozí velikost pro odpovědi
    buttonText.setCharacterSize(characterSize);
    buttonText.setString(text);

    // Získáme rozměry tlačítka
    float containerWidth = shape.getSize().x - 20.0f; // -20px jako vnitřní okraj (padding)
    
    // --- ALGORITMUS PRO ZALAMOVÁNÍ TEXTU (WORD WRAP) ---
    std::wstring wrappedText = text;
    std::wstring currentLine;
    std::wstring finalString;
    
    // Rozdělíme text na slova
    std::size_t startPos = 0;
    std::size_t spacePos = wrappedText.find(L' ');

    while (spacePos != std::wstring::npos || startPos < wrappedText.length())
    {
        // Získání slova
        std::wstring word;
        if (spacePos == std::wstring::npos) {
            word = wrappedText.substr(startPos); // Poslední slovo
            startPos = wrappedText.length();
        } else {
            word = wrappedText.substr(startPos, spacePos - startPos);
            startPos = spacePos + 1;
        }

        // Zkusíme přidat slovo k aktuálnímu řádku
        std::wstring testLine = currentLine.empty() ? word : currentLine + L" " + word;
        
        // Změříme délku testovacího řádku
        buttonText.setString(testLine);
        if (buttonText.getLocalBounds().size.x > containerWidth)
        {
            // Pokud je řádek moc dlouhý, odklepneme to, co jsme měli doteď
            if (!finalString.empty()) finalString += L"\n";
            finalString += currentLine;
            
            // A aktuální slovo dáme na nový řádek
            currentLine = word;
        }
        else
        {
            // Pokud se vejde, pokračujeme
            currentLine = testLine;
        }

        // Posun na další mezeru
        spacePos = wrappedText.find(L' ', startPos);
    }
    // Přidáme zbytek
    if (!finalString.empty()) finalString += L"\n";
    finalString += currentLine;

    // Aplikujeme zalomený text
    buttonText.setString(finalString);

    // --- CENTROVÁNÍ TEXTU ---
    // Protože text může mít teď víc řádků, musíme ho znovu vycentrovat
    sf::FloatRect bounds = buttonText.getLocalBounds();
    buttonText.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });

    sf::Vector2f btnPos = shape.getPosition();
    sf::Vector2f btnSize = shape.getSize();

    // Vycentrujeme do středu tlačítka (bez posunu -5, protože u více řádků by to dělalo neplechu)
    buttonText.setPosition({
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

void Button::moveText(float x, float y)
{
    buttonText.move({x,y});
}