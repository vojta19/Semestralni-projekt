#pragma once
#include "Button.h"
#include "AudioManager.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct Question
{
    std::wstring text;
    std::vector<std::wstring> answers;
    int correctIndex;
};

class GamePlayScreen
{
public:
    GamePlayScreen(float width, float height, const sf::Font&font);

    void startNewGame(std::wstring category, std::wstring difficulty);

    void draw(sf::RenderWindow&window);

    int handleInput(sf::RenderWindow&window, AudioManager&audio);

    void update(sf::Time deltaTime);

    void recalculatePosition(float width, float height);

private:
    sf::Font font;
    float windowWidth, windowHeight;

    std::vector<Question> questions;
    int currentQuestionIndex;
    int score;
    bool isGameOver;

    float timeLimit;
    float remainingTime;
    bool timerBlinkState;
    float blinkTimer;

    sf::Text textQuestion;
    sf::Text textTimer;
    sf::Text textCounter;

    Button btnAnswer0;
    Button btnAnswer1;
    Button btnAnswer2;
    Button btnAnswer3;

    sf::Text textGameOverTitle;
    sf::Text textScore;
    sf::Text textPercentage;
    sf::Text textThanks;
    Button btnBackToMenu;

    void loadQuestions(std::wstring category);
    void setTimeForDifficulty(std::wstring difficulty);
    void loadNextQuestionUI();
    void finishGame();

};
