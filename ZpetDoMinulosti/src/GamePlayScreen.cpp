#include "GamePlayScreen.h"
#include <iostream>
#include <cmath>

GamePlayScreen::GamePlayScreen(float width, float height, const sf::Font&font)
:font(font), windowHeight(height), windowWidth(width),

textQuestion(font),
textTimer(font),
textCounter(font),
textGameOverTitle(font),
textScore(font),
textPercentage(font),
textThanks(font),

btnAnswer0(0,0,300,50,L"",font),
btnAnswer1(0,0,300,50,L"",font),
btnAnswer2(0,0,300,50,L"",font),
btnAnswer3(0,0,300,50,L"",font),

btnBackToMenu(0,0,300,50,L"Do hlavní nabídky",font)

{
    textQuestion.setFont(font);
    textQuestion.setCharacterSize(30);
    textQuestion.setFillColor(sf::Color::White);

    textTimer.setFont(font);
    textTimer.setCharacterSize(40);
    textTimer.setFillColor(sf::Color::White);

    textCounter.setFont(font);
    textCounter.setCharacterSize(30);
    textCounter.setFillColor(sf::Color(0,255,255));

    textGameOverTitle.setFont(font);
    textGameOverTitle.setString(L"KONEC HRY");
    textGameOverTitle.setCharacterSize(60);
    textGameOverTitle.setFillColor(sf::Color::White);
    textGameOverTitle.setStyle(sf::Text::Bold);

    textScore.setFont(font);
    textScore.setCharacterSize(40);

    textPercentage.setFont(font);
    textPercentage.setCharacterSize(40);

    textThanks.setFont(font);
    textThanks.setCharacterSize(30);
    textThanks.setString(L"Děkujeme za hru! ");
    textThanks.setFillColor(sf::Color::Yellow);

    isGameOver = false;
    recalculatePosition(width,height);
}

void GamePlayScreen::setTimeForDifficulty(std::wstring difficulty)
{
    if(difficulty == L"Lehká") timeLimit = 30.0f;
    else if (difficulty == L"Střední") timeLimit = 15.0f;
    else if (difficulty == L"Těžká") timeLimit = 6.0f;
    else timeLimit = 5.0f;
}

void GamePlayScreen::startNewGame(std::wstring category, std::wstring difficulty)
{
    isGameOver = false;
    score = 0;
    currentQuestionIndex = 0;

    setTimeForDifficulty(difficulty);
    loadQuestions(category);
    loadNextQuestionUI();
}

void GamePlayScreen::loadQuestions(std::wstring category)
{
    questions.clear();
    for(int i=0;i<30;i++)
    {
        Question q;
        q.text = L"Otázka č. " + std::to_wstring(i+1) + L" / " + std::to_wstring(questions.size());
        q.answers = {L"Odpověď A", L"Odpověď B", L"Odpověď C", L"Odpověď D"};
        q.correctIndex = 0;
        questions.push_back(q); 
    }
}

void GamePlayScreen::loadNextQuestionUI()
{
    if(currentQuestionIndex >= questions.size())
    {
        finishGame();
        return;
    }

    remainingTime = timeLimit;
    Question&q = questions[currentQuestionIndex];

    textQuestion.setString(q.text);

    btnAnswer0.setText(q.answers[0]);
    btnAnswer1.setText(q.answers[1]);
    btnAnswer2.setText(q.answers[2]);
    btnAnswer3.setText(q.answers[3]);

    textCounter.setString(std::to_wstring(currentQuestionIndex+1) + L" / " + std::to_wstring(questions.size()));

    recalculatePosition(windowWidth, windowHeight);
}

void GamePlayScreen::update(sf::Time deltaTime)
{
    if(isGameOver) return;
    remainingTime -= deltaTime.asSeconds();

    int seconds = static_cast<int>(remainingTime);
    int decis = static_cast<int>((remainingTime - seconds) * 10);
    if (remainingTime < 0) remainingTime = 0;

    textTimer.setString(std::to_wstring(seconds) + L"." + std::to_wstring(decis) + L" s");

    if (remainingTime < 5.0f)
    {
        blinkTimer += deltaTime.asSeconds();
        if(blinkTimer >= 0.2f)
        {
            blinkTimer = 0;
            timerBlinkState = !timerBlinkState;
            textTimer.setFillColor(timerBlinkState ? sf::Color::Red : sf::Color::White);
        }
    }
    else
    {
        textTimer.setFillColor(sf::Color::White);
    }

    if(remainingTime <= 0.0f)
    {
        currentQuestionIndex ++;
        loadNextQuestionUI();
    }
}

void GamePlayScreen::finishGame()
{
    isGameOver = true;
    textScore.setString(L"Skóre: " + std::to_wstring(score) + L" / " + std::to_wstring(questions.size()));
    float percentage = (static_cast<float>(score) / questions.size()) * 100.0f;
    textPercentage.setString(L"Úspěšnost: " + std::to_wstring((int)percentage) + L" %");

    recalculatePosition(windowWidth,windowHeight);
}

void GamePlayScreen::recalculatePosition(float width, float height)
{
    windowWidth = width;
    windowHeight = height;

    if(!isGameOver)
    {
        sf::FloatRect qRect = textQuestion.getLocalBounds();
        textQuestion.setOrigin({qRect.position.x + qRect.size.x / 2.0f, qRect.position.y + qRect.size.y / 2.0f});
        textQuestion.setPosition({width / 2.0f,150.0f});

        sf::FloatRect tRect = textTimer.getLocalBounds();
        textTimer.setOrigin({tRect.position.x + tRect.size.x, 0.0f});
        textTimer.setPosition({width - 20.0f, 20.0f});

        textCounter.setPosition({20.0f,20.0f});

        float centerX = width / 2.0f;
        float startY = 300.0f;
        float gapX = 320.0f; // Rozestup mezi sloupci
        float gapY = 80.0f;  // Rozestup mezi řádky

        btnAnswer0.setPosition(centerX - gapX/2.0f - 150.0f, startY);
        btnAnswer1.setPosition(centerX + gapX/2.0f - 150.0f, startY);
        
        btnAnswer2.setPosition(centerX - gapX/2.0f - 150.0f, startY + gapY);
        btnAnswer3.setPosition(centerX + gapX/2.0f - 150.0f, startY + gapY);
    }
    else
    {
        sf::FloatRect titleRect = textGameOverTitle.getLocalBounds();
        textGameOverTitle.setOrigin({titleRect.position.x + titleRect.size.x / 2.0f, 0});
        textGameOverTitle.setPosition({width / 2.0f, 100.0f});

        sf::FloatRect scoreRect = textScore.getLocalBounds();
        textScore.setOrigin({scoreRect.position.x + scoreRect.size.x / 2.0f, 0});
        textScore.setPosition({width / 2.0f, 250.0f});

        sf::FloatRect percRect = textPercentage.getLocalBounds();
        textPercentage.setOrigin({percRect.position.x + percRect.size.x / 2.0f, 0});
        textPercentage.setPosition({width / 2.0f, 320.0f});

        sf::FloatRect thanksRect = textThanks.getLocalBounds();
        textThanks.setOrigin({thanksRect.position.x + thanksRect.size.x / 2.0f, 0});
        textThanks.setPosition({width / 2.0f, 400.0f});

        btnBackToMenu.setPosition(width / 2.0f - 150.0f, 500.0f);
    }
}

void GamePlayScreen::draw(sf::RenderWindow&window)
{
    if(!isGameOver)
    {
        window.draw(textQuestion);
        window.draw(textTimer);
        window.draw(textCounter);
        btnAnswer0.draw(window);
        btnAnswer1.draw(window);
        btnAnswer2.draw(window);
        btnAnswer3.draw(window);  
    }
    else
    {
        window.draw(textGameOverTitle);
        window.draw(textScore);
        window.draw(textPercentage);
        window.draw(textThanks);
        btnBackToMenu.draw(window);
    }
}

int GamePlayScreen::handleInput(sf::RenderWindow&window, AudioManager&audio)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos); 

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if(isGameOver)
        {
            if(btnBackToMenu.isClicked(mousePos))
            {
                audio.PlayClick();
                return 1;
            }
        }
        else
        {
            // Kontrola odpovědí
            // (Zjednodušeno: kontrolujeme, na co klikl a porovnáme s correctIndex)
            int clickedIndex = -1;

            if (btnAnswer0.isClicked(mousePos)) clickedIndex = 0;
            else if (btnAnswer1.isClicked(mousePos)) clickedIndex = 1;
            else if (btnAnswer2.isClicked(mousePos)) clickedIndex = 2;
            else if (btnAnswer3.isClicked(mousePos)) clickedIndex = 3;

            if (clickedIndex != -1)
            {
                // Kliknuto na odpověď
                audio.PlayClick();
                
                // Je správná?
                if (clickedIndex == questions[currentQuestionIndex].correctIndex)
                {
                    score++;
                }

                // Jdeme na další otázku
                currentQuestionIndex++;
                loadNextQuestionUI();
            }
        }
    }
    return 0;
}