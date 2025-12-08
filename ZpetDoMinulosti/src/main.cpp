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
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), L"Zpět do minulosti");
    window.setFramerateLimit(60);

    
    AudioManager audio;
    if (!audio.loadAssets())
    {
        std::cerr << "Varovani: Nepodarilo se nacist zvuky" << std::endl;
    }
    audio.playMusic();

    
    sf::Font font;
    
    if (!font.openFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/font.ttf"))
    {
        
        if (!font.openFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/font.ttf"))
        {
            std::cerr << "CHYBA: Font nenalezen!" << std::endl;
            return -1; 
        }
    }

    
    MenuScreen menu(800, 600, font);
    SettingsScreen settings(800, 600, font);

    GameState currentState = MENU;
    bool mouseClickedReleased = true;

    
    while (window.isOpen())
    {
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                sf::Vector2f newSize = static_cast<sf::Vector2f>(resized->size);
                sf::View view(sf::FloatRect({0.f, 0.f}, {newSize.x, newSize.y}));
                window.setView(view);
                if (currentState == MENU)
                {
                    menu.recalculatePosition(newSize.x, newSize.y); // Tuto metodu musíte přidat do MenuScreen!
                }
                else if (currentState == SETTINGS)
                {
                    settings.recalculatePosition(newSize.x, newSize.y);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>())
            {
                mouseClickedReleased = true;
            }
        }

        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseClickedReleased)
        {
            if (currentState == MENU)
            {
                int action = menu.handleInput(window);

                if (action != 0)
                {
                    mouseClickedReleased = false;
                    audio.PlayClick();
                }

                if (action == 1) // START
                {
                    std::cout << "Tlacitko START: Hra se spusti az dodelam GameScreen" << std::endl;
                }
                else if (action == 2) // NASTAVENÍ
                {
                    currentState = SETTINGS;
                }
                else if (action == 3) // UKONČIT
                {
                    window.close();
                }
            }
            else if (currentState == SETTINGS)
            {
                // Předáváme audio manager, aby tlačítka v nastavení mohla dělat zvuky
                bool goBack = settings.handleInput(window, audio);
                
                if (goBack)
                {
                    currentState = MENU;
                    mouseClickedReleased = false;
                    audio.PlayClick();
                }
            }
        }

        
        window.clear(sf::Color(30, 30, 30));

        if (currentState == MENU)
        {
            menu.draw(window);
        }
        else if (currentState == SETTINGS)
        {
            settings.draw(window);
        }

        window.display();
    }

    return 0;
}
