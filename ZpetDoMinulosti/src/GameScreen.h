#pragma once
#include "Button.h"
#include "AudioManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameScreen
{
public:
    GameScreen(float width, float height, const sf::Font&font);

    void draw(sf::RenderWindow&window);

    int handleIndput(sf::RenderWindow&window, AudioManager&audio);

    void recalculatePosition(float width, float height);

    std::wstring getSelectedCategory();
    std::wstring getSelectedDifficulty();

private:
    sf::Text titleText;
    Button btnCategory;
    Button btnBack;
    Button btnStart;
    Button btnDifficulty;

    std::vector<std::wstring> categories;
    std::vector<std::wstring> difficulties;

    int currentCategoryIndex;
    int currentDifficultyIndex;

    void updateButtonLabels();
};