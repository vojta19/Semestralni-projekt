#include <string>
#include "SettingScreen.h"

SettingsScreen::SettingsScreen(float width,float height, const sf::Font&font)
:titleText(font),
btnToggle(0,0,300,50,"Hudba ZAP/VYP",font),
btnResolution(0,0,300,50,"Rozlišení: 800x600",font),
btnBack(0,0,150,40,"Zpět",font)

{
    //nadpis
    titleText.setString("NASTAVENÍ");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin
    (
       {
            textRect.size.x/2.0f,
            textRect.size.y/2.0f
       } 
    );

    resolutions.push_back({800,600});
    resolutions.push_back({1280,720});
    resolutions.push_back({1366,768});
    resolutions.push_back({1920,1080});
    resolutions.push_back({2560,1440});

    currentResIndex=0;              //zaciname na 800x600
    updateResolutionButtonText();

    recalculatePosition(width,height);  //srovnani prvku na stred (poprve)
}

void SettingsScreen::recalculatePosition(float width,float height)
{
    //nadpis stred sirky, 100px od vrchu
    titleText.setPosition({width/2.0f,100.0f});


    float centerX = width /2.0f-150.0f;

    btnToggle.setPosition(centerX,200.0f);
    btnResolution.setPosition(centerX,300.0f);

    btnBack.setPosition(width-170.0f,height-60.0f);
}

void SettingsScreen::updateResolutionButtonText()
{
    sf::Vector2u res= resolutions[currentResIndex];
    std::string text= "Rozlišení: " + std::to_string(res.x) + "x" + std::to_string(res.y);
    btnResolution.setText(text);
}

void SettingsScreen::draw(sf::RenderWindow&window)
{
    window.draw(titleText);
    btnBack.draw(window);
    btnResolution.draw(window);
    btnToggle.draw(window);
}

bool SettingsScreen::handleInput(sf::RenderWindow&window, AudioManager&audio)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (btnBack.isClicked(mousePos))
        {
            audio.PlayClick();
            return true;
        }

        if (btnToggle.isClicked(mousePos))
        {
            audio.PlayClick();
            audio.toggleMusic();
        }

        if (btnResolution.isClicked(mousePos))   //zmena rozliseni 
        {
            audio.PlayClick();

            currentResIndex++;                  //dalsi index
            if (currentResIndex >= resolutions.size())
            {
                currentResIndex=0;
            }

            sf::Vector2u newRes = resolutions[currentResIndex];  //nova rozliseni

            window.setSize(newRes);         //aplikace na okno 

            sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(newRes.x), static_cast<float>(newRes.y)})); //novy pohled o velikosti okna
            window.setView(view);

            updateResolutionButtonText();   //aktualizace textu

            recalculatePosition(static_cast<float>(newRes.x), static_cast<float>(newRes.y)); //pozice tlacitek na novy stred
        }
    }
    
    return false;
}