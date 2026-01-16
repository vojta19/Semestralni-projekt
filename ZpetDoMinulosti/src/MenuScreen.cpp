#include "MenuScreen.h"
#include <iostream>

MenuScreen::MenuScreen(float width, float height, const sf::Font&font) 
:titleText(),
btnPlay(0,0,300,50,L"Hrej!",font), 
btnSettings(0,0,300,50,L"Nastavení",font),      
btnExit(0,0,300,50,L"Ukončit",font),
logoSprite(logoTexture)

{
    titleText.setFont(font);
    titleText.setString(L"Zpět do minulosti");
    titleText.setCharacterSize(85);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    std::string pathPrefix = "assets/";
    
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
	float centerX = width / 2.0f;
    sf::FloatRect titleRect = titleText.getLocalBounds();
    
    titleText.setOrigin
    ({
        titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f
    });
    
    titleText.setPosition({centerX,height*0.22f});

    float buttonHalfWidth = 150.0f;
    float btnX = centerX - buttonHalfWidth;
	float groupCenterY = height * 0.55f;
	float gap = 80.0f;
	float middleBtnY = groupCenterY-25.0f;

    btnPlay.setPosition(btnX,middleBtnY-gap);     
    btnSettings.setPosition(btnX,middleBtnY);
    btnExit.setPosition(btnX,middleBtnY+gap);

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
