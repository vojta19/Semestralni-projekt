#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include "AudioManager.h"
#include "MenuScreen.h"
#include "SettingScreen.h"
#include "GameScreen.h"
#include "GamePlayScreen.h"

enum GameState
{
    MENU,
    SETTINGS,
    GAME,
    PLAYING
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
    GameScreen game(800,600,font);
    GamePlayScreen playing (800,600,font);

    GameState currentState = MENU;
    bool mouseClickedReleased = true;
    sf::Clock gameClock;

    
    while (window.isOpen())
    {

        sf::Time deltaTime = gameClock.restart();
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                mouseClickedReleased = true;
            }
            if(const auto* resized = event->getIf<sf::Event::Resized>())
            {
                sf::Vector2f newSize = static_cast<sf::Vector2f>(resized->size);
                if (currentState == PLAYING) playing.recalculatePosition(newSize.x, newSize.y);
            }
            
            if (event->is<sf::Event::MouseButtonReleased>()) mouseClickedReleased = true;
        }

        if (currentState == PLAYING)
        {
            playing.update(deltaTime);
        }

        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseClickedReleased)
        {
            mouseClickedReleased = false;
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
                    currentState = GAME;
                    sf::Vector2u currentSize =window.getSize();
                    game.recalculatePosition((float)currentSize.x,(float)currentSize.y);
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
                    
                    sf::Vector2u currentSize = window.getSize();
                    float width = static_cast<float>(currentSize.x);
                    float height = static_cast<float>(currentSize.y);

                    sf::View view(sf::FloatRect({0.f,0.f}, {(float)currentSize.x, (float)currentSize.y}));
                    window.setView(view);

                    menu.recalculatePosition((float)currentSize.x,(float)currentSize.y);

                    
                    mouseClickedReleased = false;
                    audio.PlayClick();
                }
            }
            else if (currentState == GAME)
            {
                int action = game.handleIndput(window,audio);
                if(action == 1)
                {
                    currentState = MENU;
                    sf::Vector2u currentSize = window.getSize();
                    menu.recalculatePosition((float)currentSize.x,(float)currentSize.y);
                    audio.PlayClick();
                }
                else if (action == 2)
                {
                    std::wstring cat = game.getSelectedCategory();
                    std::wstring diff = game.getSelectedDifficulty();

                    playing.startNewGame(cat,diff);
                    currentState = PLAYING;
                    
                    sf::Vector2u currentSize = window.getSize();
                    playing.recalculatePosition((float)currentSize.x,(float)currentSize.y);
                }
            }
            else if (currentState == PLAYING)
            {
                int action = playing.handleInput(window,audio);
                if(action == 1)
                {
                    currentState = MENU;
                    sf::Vector2u currentSize = window.getSize();
                    menu.recalculatePosition((float)currentSize.x,(float)currentSize.y);
                    audio.PlayClick();
                }
                else if(action==2)
                {
                    std::wstring cat = game.getSelectedCategory();
                    std::wstring diff = game.getSelectedDifficulty();

                    playing.startNewGame(cat,diff);
                }
            }

        }

        
        window.clear(sf::Color(10, 20, 35));

        if (currentState == MENU)
        {
            menu.draw(window);
        }
        else if (currentState == SETTINGS)
        {
            settings.draw(window);
        }
        else if (currentState == GAME)
        {
            game.draw(window);
        }
        else if (currentState == PLAYING)
        {
            playing.draw(window);
        }

        window.display();
    }

    return 0;
}
