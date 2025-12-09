#include "MenuScreen.h"

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(font),
btnPlay(width/2-100,height/2,200,50,L"Hrej!",font), //uprostred
btnSettings(20,20,150,40,L"Nastavení",font),        //vlevo nahore
btnExit(width-170,height-60,150,40,L"Ukončit",font) //vpravo dole

{
    //nastaveni vzhledu nadpisu
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(85);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    //vycentrovani nadpisu
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin
    (
        {
            textRect.position.x + textRect.size.x/2.0f,
            textRect.position.y+textRect.size.y/2.0f
        }
    );
    titleText.setPosition({width/2.0f,100.0f});
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
    float centerX = width / 2.0f - 100.0f; 

    btnPlay.setPosition(centerX, 200.0f);     
    btnSettings.setPosition(width/2.0f - 150.0f, 300.0f);
    btnExit.setPosition(width/2.0f-100.0f, 200.0f);
}
