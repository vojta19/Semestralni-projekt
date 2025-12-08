#include "MenuScreen.h"

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(font),
btnPlay(width/2-100,height/2,200,50,L"Hrej!",font), //uprostred
btnSettings(20,20,150,40,L"Nastavení",font),        //vlevo nahore
btnExit(width-170,height-60,150,40,L"Ukončit",font) //vpravo dole

{
    //nastaveni vzhledu nadpisu
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(50);
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

// Vložte na konec souboru MenuScreen.cpp

void MenuScreen::recalculatePosition(float width, float height)
{
    // 1. Vycentrování nadpisu
    sf::FloatRect titleRect = titleText.getLocalBounds();
    
    // Výpočet pozice X: (šířka okna - šířka textu) / 2
    float titleX = (width - titleRect.size.x) / 2.0f;
    
    titleText.setPosition({titleX, 50.0f});

    // 2. Vycentrování tlačítek
    float buttonWidth = 200.0f; // Šířka tlačítek (pokud máte jinou, upravte)
    float centerX = width / 2.0f - buttonWidth / 2.0f;

    // Srovnání tlačítek pod sebe (upravte názvy proměnných btn..., pokud se jmenují jinak)
    btnPlay.setPosition(centerX, 200.0f);
    btnSettings.setPosition(centerX, 300.0f);
    btnExit.setPosition(centerX, 400.0f);
}