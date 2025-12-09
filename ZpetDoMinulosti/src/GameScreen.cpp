#include <iostream>
#include "GameScreen.h"

GameScreen::GameScreen(float width, float height, const sf::Font&font) :titleText(font),
btnCategory(0,0,300,50,L"Kategorie",font),
btnDifficulty(0,0,300,50,L"Obtížnost",font),
btnStart(0,0,300,50,L"SPUSTIT HRU",font),
btnBack(0,0,150,40,L"Zpět",font)

{
    titleText.setString(L"PŘÍPRAVA HRY");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color(0,255,255));
    titleText.setStyle(sf::Text::Bold);

    categories = {L"Starověk",L"Středověk",L"Moderní dějiny"};
    difficulties = {L"Lehká",L"Střední",L"Těžká",L"Blesková"};

    currentCategoryIndex = 0;
    currentDifficultyIndex = 1;

    updateButtonLabels();
    recalculatePosition(width,height);
}

void GameScreen::updateButtonLabels()
{
    btnCategory.setText(L"Kategorie " + categories[currentCategoryIndex]);
    btnDifficulty.setText(L"Obtížnost " + difficulties[currentDifficultyIndex]);
}

void GameScreen::recalculatePosition(float width, float height)
{
        // 1. Vycentrování nadpisu
    sf::FloatRect titleRect = titleText.getLocalBounds();
    
    // SFML 3.0 syntaxe:
    titleText.setOrigin
    ({
        titleRect.position.x + titleRect.size.x / 2.0f,
        titleRect.position.y + titleRect.size.y / 2.0f
    });
    
    // Pozice: střed šířky, 100px od vrchu
    titleText.setPosition({width / 2.0f, 100.0f});

    // 2. Vycentrování tlačítek
    float buttonHalfWidth = 150.0f;
    float centerX = width / 2.0f - buttonHalfWidth; 

    btnCategory.setPosition(centerX, 250.0f);     
    btnDifficulty.setPosition(centerX, 350.0f);
    btnStart.setPosition(centerX, 500.0f);

    btnBack.setPosition(width-170.0f,height-60.0f);
}

void GameScreen::draw(sf::RenderWindow&window)
{
    window.draw(titleText);
    btnCategory.draw(window);
    btnDifficulty.draw(window);
    btnStart.draw(window);
    btnBack.draw(window);
}

std::wstring GameScreen::getSelectedCategory()
{
    return categories[currentCategoryIndex];
}

std::wstring GameScreen::getSelectedDifficulty()
{
    return difficulties[currentDifficultyIndex];
}

int GameScreen::handleIndput(sf::RenderWindow&window, AudioManager&audio)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if(btnCategory.isClicked(mousePos))
        {
            audio.PlayClick();
            currentCategoryIndex++;
            if(currentCategoryIndex>= categories.size())
            {
                currentCategoryIndex = 0;
            }
            updateButtonLabels();
        }


        if(btnDifficulty.isClicked(mousePos))
        {
            audio.PlayClick();
            currentDifficultyIndex++;
            if(currentDifficultyIndex >= difficulties.size())
            {
                currentDifficultyIndex = 0;
            }
            updateButtonLabels();
        }

        if(btnStart.isClicked(mousePos))
        {
            audio.PlayClick();
            return 2;
        }

        if(btnBack.isClicked(mousePos))
        {
            audio.PlayClick();
            return 1;
        }
    }
    return 0;
}