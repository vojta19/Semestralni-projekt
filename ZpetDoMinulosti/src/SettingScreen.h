#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"
#include "AudioManager.h" 

class SettingsScreen
{
public:
    SettingsScreen(float width, float height, const sf::Font& font);

    void draw(sf::RenderWindow& window);

    bool handleInput(sf::RenderWindow& window, AudioManager& audio);

    void recalculatePosition(float width, float height);

private:
    sf::Text titleText;       

    Button btnToggle;        
    Button btnResolution;     
    Button btnBack;           

    std::vector<sf::Vector2u> resolutions; 
    int currentResIndex;                   

    void updateResolutionButtonText();
};