#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"
#include "AudioManager.h"

class SettingsScreen
{
public:
    SettingsScreen(float width,float height, const sf::Font&font);
    void draw(sf::RenderWindow&window);
    bool handleInput(sf::RenderWindow&window, AudioManager&audio);

private:
    sf::Text titleText;

    Button btnToggle;     //pro vypnuti/zapnuti hudby
    Button btnResolution; //pro zmenu rozliseni
    Button btnBack;       //pro cestu zpet

    std::vector<sf::Vector2u>resolutions; //seznam moznosti
    int currentResIndex;                    //jake je vybrane

    void updateResolutionButtonText(); //pomocna funkce pro zmenu textu na tlacitku

    void recalculatePosition(float width,float height); //srovna tlacitka nastred podle nove sirky/vysky

};