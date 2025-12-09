#include "MenuScreen.h"

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(font),
btnPlay(0,0,300,50,L"Hrej!",font), //uprostred
btnSettings(0,0,300,50,L"Nastavení",font),        //vlevo nahore
btnExit(0,0,300,50,L"Ukončit",font) //vpravo dole

{
    //nastaveni vzhledu nadpisu
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(85);
    titleText.setFillColor(sf::Color(0,255,255));
    titleText.setStyle(sf::Text::Bold);

    //vycentrovani nadpisu
   // sf::FloatRect textRect = titleText.getLocalBounds();
   // titleText.setOrigin
   // (
   //     {
   //         textRect.position.x + textRect.size.x/2.0f,
   //         textRect.position.y+textRect.size.y/2.0f
   //     }
   // );
   // titleText.setPosition({width/2.0f,100.0f});

   recalculatePosition(width,height);
}

//vykresleni
void MenuScreen::draw(sf::RenderWindow&window)
{
    window.draw(titleText);
    btnPlay.draw(window);
    btnExit.draw(window);
    btnSettings.draw(window);
}

int MenuScreen::handleInput(sf::RenderWindow&window)
{
    //prevod pozice mysi
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

// Vložte do MenuScreen.cpp

void MenuScreen::recalculatePosition(float width, float height)
{
    // 1. Vycentrování nadpisu
    sf::FloatRect titleRect = titleText.getLocalBounds();
    
    // SFML 3.0 syntaxe:
    titleText.setOrigin
    ({
        titleRect.position.x + titleRect.size.x / 2.0f,
        titleRect.position.y + titleRect.size.y / 2.0f
    });
    
    // Pozice: střed šířky, 100px od vrchu
    titleText.setPosition({width / 2.0f, 100.0f});

    // 2. Vycentrování tlačítek
    float buttonHalfWidth = 150.0f;
    float centerX = width / 2.0f - buttonHalfWidth; 

    btnPlay.setPosition(centerX, 250.0f);     
    btnSettings.setPosition(centerX, 350.0f);
    btnExit.setPosition(centerX, 450.0f);
}
