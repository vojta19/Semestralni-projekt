#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include "AudioManager.h"
#include "MenuScreen.h"
#include "SettingScreen.h"
#include "GameScreen.h"
#include "GamePlayScreen.h"
#include "QuestionManager.h"

enum GameState
{
    MENU,
    SETTINGS,
    GAME,
    PLAYING
};

void updateViewAndLayout(sf::RenderWindow& window, auto& screen)
{
    sf::Vector2u size = window.getSize();
    float w = static_cast<float>(size.x);
    float h = static_cast<float>(size.y);

    sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(w, h));
    window.setView(sf::View(visibleArea));

    screen.recalculatePosition(w, h);
}

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
    
    if (!font.loadFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/font.ttf"))
    {
        
        if (!font.loadFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/font.ttf"))
        {
            std::cerr << "CHYBA: Font nenalezen!" << std::endl;
            return -1; 
        }
    }

    
    MenuScreen menu(800, 600, font);
    SettingsScreen settings(800, 600, font);
    GameScreen game(800,600,font);
    GamePlayScreen playing (window,800,600,font);

    GameState currentState = MENU;
    bool mouseClickedReleased = true;
    sf::Clock gameClock;

    
    while (window.isOpen())
    {

        sf::Time deltaTime = gameClock.restart();
        sf::Event(event);
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::Closed)
            {
                window.close();
            }
            if (event.type==sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    if(currentState== PLAYING)
                    {
                        playing.togglePause();
                    }
                }
            }
            if (event.type==sf::Event::MouseButtonReleased)
            {
                mouseClickedReleased = true;
            }
            if(event.type==sf::Event::Resized)
            {
                float w = static_cast<float>(event.size.width);
                float h = static_cast<float>(event.size.height);
                
                sf::FloatRect visibleArea(0, 0, w, h);
                window.setView(sf::View(visibleArea));

                menu.recalculatePosition(w, h);
                settings.recalculatePosition(w, h);
                game.recalculatePosition(w, h);
                playing.recalculatePosition(w, h);
                
                if (currentState == PLAYING) playing.recalculatePosition(event.size.width, event.size.height);
            }
            
            if (event.type==sf::Event::MouseButtonReleased) mouseClickedReleased = true;
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

                if (action == 1) 
                {
                    currentState = GAME;
                    sf::Vector2u currentSize =window.getSize();
                    game.recalculatePosition((float)currentSize.x,(float)currentSize.y);
                    updateViewAndLayout(window,playing);
                }
                else if (action == 2) 
                {
                    currentState = SETTINGS;
                    updateViewAndLayout(window,playing);
                }
                else if (action == 3) 
                {
                    window.close();
                }
            }
            else if (currentState == SETTINGS)
            {
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
                    updateViewAndLayout(window,playing);
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
                    updateViewAndLayout(window,playing);
                }
                else if (action == 2)
                {
                    std::wstring cat = game.getSelectedCategory();
                    std::wstring diff = game.getSelectedDifficulty();

                    updateViewAndLayout(window,playing);
                    
                    playing.startNewGame(cat,diff);
                    currentState = PLAYING;

                    updateViewAndLayout(window,playing);
                    
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
                    updateViewAndLayout(window,playing);
                }
                else if(action==3)
                {
                    std::wstring cat = game.getSelectedCategory();
                    std::wstring diff = game.getSelectedDifficulty();

                    playing.startNewGame(cat,diff);
                    updateViewAndLayout(window,playing);

                    sf::Vector2u currentSize = window.getSize();
                    playing.recalculatePosition((float)currentSize.x, (float)currentSize.y);
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
