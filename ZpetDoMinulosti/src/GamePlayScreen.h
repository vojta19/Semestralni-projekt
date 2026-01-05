#pragma once
#include "Button.h"
#include "AudioManager.h"
#include "QuestionManager.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class GamePlayScreen
{
public:
    GamePlayScreen(float width, float height, const sf::Font&font);

    void startNewGame(std::wstring category, std::wstring difficulty);

    void draw(sf::RenderWindow&window);

    int handleInput(sf::RenderWindow&window, AudioManager&audio);

    void update(sf::Time deltaTime);

    void recalculatePosition(float width, float height);

    void togglePause();

private:
    sf::Font font;
    float windowWidth, windowHeight;

    std::vector<Question> questions;
    int currentQuestionIndex;
    int score;
    bool isGameOver;
    bool isPaused;

    std::wstring currentCategory;

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

    sf::RectangleShape pauseOverlay;
    sf::Text textPausedTitle;
    Button btnResume;

    sf::RectangleShape resultsPanel;
    sf::Text textGameOverTitle;
    sf::Text textScore;
    sf::Text textPercentage;
    sf::Text textRank;
   // sf::Text textThanks;
    Button btnBackToMenu;
    Button btnRestart;

    bool isShuffleActive;
    bool isFogActive;
    sf::Text eventLabel;
    sf::Clock chaosShuffleTimer;

    void loadQuestions(std::wstring category,std::wstring difficulty);
    void setTimeForDifficulty(std::wstring difficulty);
    void loadNextQuestionUI();
    void finishGame();
    void triggerChaosEvent();

};
