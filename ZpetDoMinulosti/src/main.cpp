#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include "AudioManager.h"
#include "MenuScreen.h"
#include "SettingScreen.h"


enum GameState
{
    MENU,
    SETTINGS
};



int main()
{
    sf::RenderWindow window(sf::VideoMode({800,600}),L"Zpět do minulosti");
    window.setFramerateLimit(60);

    AudioManager audio;
    if(!audio.loadAssets())
    {
        std::cerr << "Varovani: Nepodarilo se nacist zvuky" << std::endl;
    }
    audio.playMusic();

    sf::Font font;
    if(!font.openFromFile("assets/font.tff"))
    {
        if(!font.openFromFile("../assets/font.tff"))
        {
            std::cerr << "CHYBA: Font nenalezen!" << std::endl;
            return -1;
        }
    }

    MenuScreen menu(800,600,font);
    SettingsScreen settings(800,600,font);

    GameState currentState=MENU;

    bool mouseClickedReleased = true;

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event -> is<sf::Event::Closed>())
            {
                window.close();
            }

            if(event -> is<sf::Event::MouseButtonReleased>())
            {
                mouseClickedReleased=true;
            }
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseClickedReleased)
        {
            if (currentState == MENU)
            {
                int action = menu.handleInput(window);

                if(action !=0)
                {
                    mouseClickedReleased= false;
                    audio.PlayClick();
                }
                if(action ==1)
                {
                    std::cout << "Tlacitko START: Hra se spusti az dodelam GameScreen" << std::endl;
                }
                else if (action ==2)
                {
                    currentState= SETTINGS;
                }
                else if(action == 3)
                {
                    window.close();
                }
            }

            else if(currentState = SETTINGS)
            {
                bool goBack= settings.handleInput(window,audio);
                if(goBack)
                {
                    currentState = MENU;
                    mouseClickedReleased = false;
                    audio.PlayClick();
                }
            }
        }
        window.clear(sf::Color(30,30,30));
        if(currentState = MENU)
        {
            menu.draw(window);
        }
        else if(currentState = SETTINGS)
        {
            settings.draw(window);
        }
        window.display();
    }

    return 0;
}
