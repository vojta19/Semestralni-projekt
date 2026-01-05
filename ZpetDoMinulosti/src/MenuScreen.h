#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class MenuScreen
{
public:
    MenuScreen(float widht, float height, const sf::Font&font);
    
    void draw(sf::RenderWindow& window);                    
    
    int handleInput(sf::RenderWindow&window);

    void recalculatePosition(float width, float height);

private:
    sf::Text titleText; 

    Button btnPlay;    
    Button btnSettings; 
    Button btnExit;     
};

