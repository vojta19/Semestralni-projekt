#include "MenuScreen.h"

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(font),
btnPlay(0,0,300,50,L"Hrej!",font), 
btnSettings(0,0,300,50,L"Nastavení",font),      
btnExit(0,0,300,50,L"Ukončit",font) 

{
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(85);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

   recalculatePosition(width,height);
}

void MenuScreen::draw(sf::RenderWindow&window)
{
    window.draw(titleText);
    btnPlay.draw(window);
    btnExit.draw(window);
    btnSettings.draw(window);
}

int MenuScreen::handleInput(sf::RenderWindow&window)
{
    sf::Vector2i pixelPos=sf::Mouse::getPosition(window);
    sf::Vector2f mousePos=window.mapPixelToCoords(pixelPos);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (btnPlay.isClicked(mousePos)) return 1;
        if (btnExit.isClicked(mousePos)) return 3;
        if (btnSettings.isClicked(mousePos)) return 2;
    }

    return 0;
}


void MenuScreen::recalculatePosition(float width, float height)
{
    sf::FloatRect titleRect = titleText.getLocalBounds();
    
    titleText.setOrigin
    ({
        titleRect.position.x + titleRect.size.x / 2.0f,
        titleRect.position.y + titleRect.size.y / 2.0f
    });
    
    titleText.setPosition({width / 2.0f, 100.0f});

    float buttonHalfWidth = 150.0f;
    float centerX = width / 2.0f - buttonHalfWidth; 

    btnPlay.setPosition(centerX, 250.0f);     
    btnSettings.setPosition(centerX, 350.0f);
    btnExit.setPosition(centerX, 450.0f);
}
