#include "MenuScreen.h"
#include <iostream>

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(font),
btnPlay(0,0,300,50,L"Hrej!",font), 
btnSettings(0,0,300,50,L"Nastavení",font),      
btnExit(0,0,300,50,L"Ukončit",font),
logoSprite(logoTexture)

{
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(85);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    std::string pathPrefix = "/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/";
    
    if (!logoTexture.loadFromFile(pathPrefix + "logo.png"))
    {
        if (!logoTexture.loadFromFile("../ZpetDoMinulosti/assets/logo.png"))
        {
            if (!logoTexture.loadFromFile("assets/logo.png"))
            {
                std::cerr << "Varovani: Logo 'logo.png' nenalezeno v assets!" << std::endl;
            }
        }
    }

    logoTexture.setSmooth(true);
    if(logoTexture.getSize().x>0)
    {
        logoSprite.setTexture(logoTexture,true);
    }
    recalculatePosition(width,height);
}

void MenuScreen::draw(sf::RenderWindow&window)
{
    window.draw(titleText);
    btnPlay.draw(window);
    btnSettings.draw(window);
    btnExit.draw(window);

    if(logoTexture.getSize().x>0)
    {
        window.draw(logoSprite);
    }
}

int MenuScreen::handleInput(sf::RenderWindow&window)
{
    sf::Vector2i pixelPos=sf::Mouse::getPosition(window);
    sf::Vector2f mousePos=window.mapPixelToCoords(pixelPos);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (btnPlay.isClicked(mousePos)) return 1;
        if (btnSettings.isClicked(mousePos)) return 2;
        if (btnExit.isClicked(mousePos)) return 3;

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

    if(logoTexture.getSize().x>0)
    {
        sf::Vector2u texSize=logoTexture.getSize();
        logoSprite.setOrigin({texSize.x/2.0f,texSize.y/2.0f});
        logoSprite.setPosition({width*0.85f,height*0.5f});

        float desiredHeight=height*0.35f;
        float scaleFactor = desiredHeight/texSize.y;

        logoSprite.setScale({scaleFactor,scaleFactor});
    }
}
