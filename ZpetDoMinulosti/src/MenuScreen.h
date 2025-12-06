#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class MenuScreen
{
public:
    //konstruktor
    MenuScreen(float widht, float height, const sf::Font&font);
    
    //vykresleni
    void draw(sf::RenderWindow& window);                    
    
    //zpracovani kliknuti
    int handleInput(sf::RenderWindow&window);

private:
    sf::Text titleText; //nadpis Zpet do minulosti

    Button btnPlay;    //Hrej!
    Button btnSettings; //Nastaveni
    Button btnExit;     //Ukoncit
};

