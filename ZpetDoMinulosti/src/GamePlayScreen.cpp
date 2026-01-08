#include "GamePlayScreen.h"
#include <iostream>
#include <cmath>
#include "QuestionManager.h"
#include <random>
#include <future>
#include <thread>
#include <optional>

GamePlayScreen::GamePlayScreen(sf::RenderWindow&window,float width, float height, const sf::Font&font)
:windowRef(&window),
font(font), windowHeight(height), windowWidth(width),

textQuestion(font),
textTimer(font),
textCounter(font),
textGameOverTitle(font),
textScore(font),
textPercentage(font),
textRank(font),
textPausedTitle(font),
eventLabel(font),

btnAnswer0(0,0,300,50,L"",font),
btnAnswer1(0,0,300,50,L"",font),
btnAnswer2(0,0,300,50,L"",font),
btnAnswer3(0,0,300,50,L"",font),
btnResume(0,0,300,50,L"Zpět do hry",font),

btnBackToMenu(0,0,300,50,L"Do hlavní nabídky",font),
btnRestart(0,0,300,50,L"Zkusit znovu",font)

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

    resultsPanel.setFillColor(sf::Color(0,0,0,150));
    resultsPanel.setOutlineColor(sf::Color(0,255,255));
    resultsPanel.setOutlineThickness(2);

    textRank.setFont(font);
    textRank.setCharacterSize(50);
    textRank.setStyle(sf::Text::Bold);

    pauseOverlay.setFillColor(sf::Color(0,0,0,240));
    textPausedTitle.setString(L"Hra pozastavena");
    textPausedTitle.setCharacterSize(50);
    textPausedTitle.setFillColor(sf::Color::White);
    textPausedTitle.setStyle(sf::Text::Bold);

    eventLabel.setCharacterSize(28);
    eventLabel.setStyle(sf::Text::Bold);

    isGameOver = false;
    isPaused = false;
    skipFirstUpdate = false;

    isTransitioning = false;

    isShuffleActive=false;
    isFogActive=false;

    recalculatePosition(width,height);
}

void GamePlayScreen::togglePause()
{
    if(!isGameOver)
    {
        isPaused=!isPaused;
        recalculatePosition(windowWidth,windowHeight);
    }
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
    isPaused = false;
    score = 0;
    currentQuestionIndex = 0;
    currentCategory=category;

    skipFirstUpdate = true;

    setTimeForDifficulty(difficulty);
    loadQuestions(category, difficulty);
    loadNextQuestionUI();
}

void GamePlayScreen::loadQuestions(std::wstring category, std::wstring difficulty)
{
   questions.clear();

    std::atomic<int> loadedCount(0);
    int totalCount = 30;

    sf::Text loadingText(font);
    loadingText.setCharacterSize(40);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setStyle(sf::Text::Bold);

    auto future = std::async(std::launch::async, 
    QuestionManager::fetchQuestions, category, difficulty, &loadedCount);

    while (future.wait_for(std::chrono::milliseconds(16)) != std::future_status::ready)
    {
        if (windowRef) 
        {
            while (const auto event = windowRef->pollEvent()) 
            {
                if (event->is<sf::Event::Closed>()) 
                {
                    windowRef->close();
                    return; 
                }
            }
        }

        if (windowRef && windowRef->isOpen())
        {
            windowRef->clear(sf::Color::Black); 

            sf::Vector2u winSize = windowRef->getSize();
            float w = static_cast<float>(winSize.x);
            float h = static_cast<float>(winSize.y);

            sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(w, h));
            windowRef->setView(sf::View(visibleArea));

            windowWidth = w;
            windowHeight = h;

            std::wstring msg = L"NAČÍTÁM OTÁZKY: " + std::to_wstring(loadedCount) + L" / " + std::to_wstring(totalCount);
            loadingText.setString(msg);

            sf::FloatRect r = loadingText.getLocalBounds();
            loadingText.setOrigin({r.position.x + r.size.x/2.0f, r.position.y + r.size.y/2.0f});
            loadingText.setPosition({w / 2.0f, h / 2.0f});

            windowRef->draw(loadingText);
            windowRef->display();
        }
    }

    std::vector<Question> downloaded = future.get();

    if (!downloaded.empty())
    {
        questions = downloaded;
    }
    else
    {       
        for (int i = 0; i < 5; i++)
        {
            Question q;
            q.text = L"Chyba připojení k internetu. (Záložní otázka " + std::to_wstring(i + 1) + L")";
            q.answers = {L"Odpověď A", L"Odpověď B", L"Odpověď C", L"Odpověď D"};
            q.correctIndex = 0;
            questions.push_back(q);
        }
    }

    if (windowRef) 
    {
        sf::Vector2u size = windowRef->getSize();
        sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)size.x, (float)size.y));
        windowRef->setView(sf::View(visibleArea));
        
        windowWidth = (float)size.x;
        windowHeight = (float)size.y;
    }
}

void GamePlayScreen::triggerChaosEvent()
{
    isShuffleActive = false;
    isFogActive = false;
    eventLabel.setString("");

    int roll = rand() % 3;

    if (roll == 0) 
    {
        isShuffleActive = true;
        eventLabel.setString(L"! CHAOS: ZPŘEHÁZENÉ ODPOVĚDI !");
        eventLabel.setFillColor(sf::Color::White);
    } 
    else if (roll == 1) 
    {
        isFogActive = true;
        eventLabel.setString(L"! CHAOS: CENZURA (Najeď myší) !");
        eventLabel.setFillColor(sf::Color::White);
    } 
    else 
    {
        isShuffleActive = true;
        isFogActive = true;
        eventLabel.setString(L"!!! TOTÁLNÍ CHAOS !!!");
        eventLabel.setFillColor(sf::Color::White);
    }

    sf::FloatRect r = eventLabel.getLocalBounds();
    eventLabel.setOrigin({r.position.x + r.size.x / 2.0f, r.position.y + r.size.y / 2.0f});
    eventLabel.setPosition({windowWidth / 2.0f, 270.0f}); 
}

void GamePlayScreen::loadNextQuestionUI()
{
    isTransitioning = false;

    sf::Color defaultColor(0,50,100);
    btnAnswer0.setBackgroundColor(defaultColor);
    btnAnswer1.setBackgroundColor(defaultColor);
    btnAnswer2.setBackgroundColor(defaultColor);
    btnAnswer3.setBackgroundColor(defaultColor);

    if (currentQuestionIndex >= questions.size()) 
    {
        finishGame();
        return;
    }

    if(currentCategory==L"Chaos")
    {
        timeLimit = 20.0f;
        remainingTime = 20.0f;
        triggerChaosEvent();
    }
    else
    {
        remainingTime = timeLimit;
        isShuffleActive = false;
        isFogActive = false;
        eventLabel.setString("");
    }

    chaosShuffleTimer.restart();

    int seconds = static_cast<int>(remainingTime);
    int decis = static_cast<int>((remainingTime - seconds) * 10);
    textTimer.setString(std::to_wstring(seconds) + L"." + std::to_wstring(decis) + L" s");

    Question& q = questions[currentQuestionIndex];

    std::wstring rawText = q.text;
    std::wstring wrappedText;
    std::wstring line;
    
    textQuestion.setString(rawText);
    textQuestion.setCharacterSize(24); 
    
    float maxWidth = windowWidth * 0.8f;

    std::size_t startPos = 0;
    std::size_t spacePos = rawText.find(L' ');
    
    while (spacePos != std::wstring::npos || startPos < rawText.length())
    {
        std::wstring word;
        if (spacePos == std::wstring::npos) {
            word = rawText.substr(startPos);
            startPos = rawText.length();
        } else {
            word = rawText.substr(startPos, spacePos - startPos);
            startPos = spacePos + 1;
        }

        std::wstring testLine = line.empty() ? word : line + L" " + word;
        textQuestion.setString(testLine);
        
        if (textQuestion.getLocalBounds().size.x > maxWidth)
        {
            if (!wrappedText.empty()) wrappedText += L"\n";
            wrappedText += line;
            line = word;
        }
        else
        {
            line = testLine;
        }
        spacePos = rawText.find(L' ', startPos);
    }
    if (!wrappedText.empty()) wrappedText += L"\n";
    wrappedText += line;

    textQuestion.setString(wrappedText);
    
    sf::FloatRect qRect = textQuestion.getLocalBounds();
    textQuestion.setOrigin({
        qRect.position.x + qRect.size.x / 2.0f,
        qRect.position.y + qRect.size.y / 2.0f
    });
    
    textQuestion.setPosition({windowWidth / 2.0f, 150.0f});

    btnAnswer0.setText(q.answers[0]);
    btnAnswer1.setText(q.answers[1]);
    btnAnswer2.setText(q.answers[2]);
    btnAnswer3.setText(q.answers[3]);

    textCounter.setString(std::to_wstring(currentQuestionIndex + 1) + L" / " + std::to_wstring(questions.size()));

    if (windowRef) 
    {
        sf::Vector2u size = windowRef->getSize();
        recalculatePosition(static_cast<float>(size.x), static_cast<float>(size.y));
    } 
    else 
    {
        recalculatePosition(windowWidth, windowHeight);
    }
}


void GamePlayScreen::update(sf::Time deltaTime)
{
    if (skipFirstUpdate)
    {
        skipFirstUpdate = false;
        return;
    }

    if(isGameOver || isPaused) return;

    if (isTransitioning)
    {
        if (transitionClock.getElapsedTime().asSeconds()> 0.5f)
        {
            currentQuestionIndex++;
            loadNextQuestionUI();
        }
        return;
    }
    remainingTime -= deltaTime.asSeconds();

    int seconds = static_cast<int>(remainingTime);
    int decis = static_cast<int>((remainingTime - seconds) * 10); 
    if (remainingTime < 0) remainingTime = 0; 
    textTimer.setString(std::to_wstring(seconds) + L"." + std::to_wstring(decis) + L" s"); 
    
    if (remainingTime < 5.0f) 
    { 
        blinkTimer += deltaTime.asSeconds(); 
        float blinkSpeed = (remainingTime<2.0f) ? 0.1f:0.2f;
        if (blinkTimer >= blinkSpeed) 
        { 
            blinkTimer = 0; 
            timerBlinkState = !timerBlinkState; 
            textTimer.setFillColor(timerBlinkState ? sf::Color::Red : sf::Color::White); 
        } 

        float pulseSpeed = 10.0f + (5.0f - remainingTime) * 2.0f;
        float scaleFactor = 1.0f + 0.15f * std::sin(remainingTime*pulseSpeed);

        textTimer.setScale({scaleFactor,scaleFactor});
    } 
    else 
    { 
        textTimer.setFillColor(sf::Color::White); 
        textTimer.setScale({1.0f,1.0f});
    }
    if (remainingTime <= 0.0f) 
    { 
        currentQuestionIndex++; loadNextQuestionUI(); 
    }

    if (isShuffleActive) 
    {
        if (chaosShuffleTimer.getElapsedTime().asSeconds() > 2.0f) 
        {
            std::vector<sf::Vector2f> positions;
            positions.push_back(btnAnswer0.getPosition());
            positions.push_back(btnAnswer1.getPosition());
            positions.push_back(btnAnswer2.getPosition());
            positions.push_back(btnAnswer3.getPosition());

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(positions.begin(), positions.end(), g);

            btnAnswer0.setPosition(positions[0]);
            btnAnswer1.setPosition(positions[1]);
            btnAnswer2.setPosition(positions[2]);
            btnAnswer3.setPosition(positions[3]);

            chaosShuffleTimer.restart();
        }
    }
}

void GamePlayScreen::finishGame()
{
    isGameOver = true; isPaused = false;
    textScore.setString(L"Skóre: " + std::to_wstring(score) + L" / " + std::to_wstring(questions.size()));
    float percentage = (static_cast<float>(score) / questions.size()) * 100.0f;
    textPercentage.setString(L"Úspěšnost: " + std::to_wstring((int)percentage) + L" %");

    if(percentage>=90.0f)
    {
        textRank.setString(L"HODNOST: PÁN ČASU");
        textRank.setFillColor(sf::Color(255,215,0));
    }
    else if(percentage >=60.0f)
    {
        textRank.setString(L"HODNOST: STRÁŽCE HISTORIE");
        textRank.setFillColor(sf::Color(0,255,255));
    }
    else if(percentage >=30.0f)
    {
        textRank.setString(L"HODNOST: ČASOVÝ TURISTA");
        textRank.setFillColor(sf::Color(255,165,0));
    }
    else
    {
        textRank.setString(L"HODNOST: ZTRACEN V ČASE");
        textRank.setFillColor(sf::Color(255,50,50));
    }

    recalculatePosition(windowWidth,windowHeight);
}

void GamePlayScreen::recalculatePosition(float width, float height)
{
    windowWidth = width;
    windowHeight = height;

    if (windowRef) 
    {
        sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(width, height));
        windowRef->setView(sf::View(visibleArea));
    }

    float centerX = width/2.0f;
    float centerY = height / 2.0f;

    if(isPaused)
    {
        pauseOverlay.setSize({width,height});
        pauseOverlay.setPosition({0,0});

        sf::FloatRect pRect = textPausedTitle.getLocalBounds();
        textPausedTitle.setOrigin({pRect.position.x + pRect.size.x / 2.0f, pRect.position.y + pRect.size.y / 2.0f});
        textPausedTitle.setPosition({centerX, height * 0.25f});

        btnResume.setPosition(centerX - 150.0f, centerY);
        btnBackToMenu.setPosition(centerX - 150.0f, centerY + 80.0f);
    }
    else if(!isGameOver && !isPaused)
    {
        sf::FloatRect qRect = textQuestion.getLocalBounds();
        textQuestion.setOrigin({qRect.position.x + qRect.size.x / 2.0f, qRect.position.y + qRect.size.y / 2.0f});
        textQuestion.setPosition({width / 2.0f,150.0f});

        if(currentCategory==L"Chaos")
        {
            sf::FloatRect eRect = eventLabel.getLocalBounds();
            eventLabel.setOrigin({eRect.position.x + eRect.size.x / 2.0f, eRect.position.y + eRect.size.y / 2.0f});
            eventLabel.setPosition({centerX, 270.0f});
        }

        sf::FloatRect tRect = textTimer.getLocalBounds();
        textTimer.setOrigin({tRect.position.x + tRect.size.x, 0.0f});
        textTimer.setPosition({width - 20.0f, 20.0f});

        textCounter.setPosition({20.0f,20.0f});

        float centerX = width / 2.0f;
        float startY = 300.0f;
        float gapX = 320.0f; 
        float gapY = 80.0f;  

        btnAnswer0.setPosition(centerX - gapX/2.0f - 150.0f, startY);
        btnAnswer1.setPosition(centerX + gapX/2.0f - 150.0f, startY);
        
        btnAnswer2.setPosition(centerX - gapX/2.0f - 150.0f, startY + gapY);
        btnAnswer3.setPosition(centerX + gapX/2.0f - 150.0f, startY + gapY);
    }
    else
    {
        float centerX = width/2.0f;
        float centerY = height/2.0f;

        resultsPanel.setSize({800.0f,450.0f});
        resultsPanel.setOrigin({400.0f,225.0f});
        resultsPanel.setPosition({centerX,centerY});

        sf::FloatRect titleRect = textGameOverTitle.getLocalBounds();
        textGameOverTitle.setOrigin({titleRect.position.x + titleRect.size.x / 2.0f, 0});
        textGameOverTitle.setPosition({centerX,centerY - 180.0f});

        sf::FloatRect rankRect = textRank.getLocalBounds();
        textRank.setOrigin({rankRect.position.x + rankRect.size.x / 2.0f,0});
        textRank.setPosition({centerX,centerY - 100.0f});

        sf::FloatRect scoreRect = textScore.getLocalBounds();
        textScore.setOrigin({scoreRect.position.x + scoreRect.size.x / 2.0f, 0});
        textScore.setPosition({centerX,centerY - 20.0f});

        sf::FloatRect percRect = textPercentage.getLocalBounds();
        textPercentage.setOrigin({percRect.position.x + percRect.size.x / 2.0f, 0});
        textPercentage.setPosition({centerX,centerY + 30.0f});

        btnBackToMenu.setPosition(centerX - 10.0f - 300.0f, centerY + 120.0f);
        btnRestart.setPosition(centerX + 10.0f,centerY + 120.0f);
        btnRestart.moveText(0.0f,-3.0f);
    }
}

void GamePlayScreen::draw(sf::RenderWindow&window)
{
    if(isPaused)
    {
        window.draw(textQuestion);
        window.draw(textTimer);
        window.draw(textCounter);
        btnAnswer0.draw(window);
        btnAnswer1.draw(window);
        btnAnswer2.draw(window);
        btnAnswer3.draw(window);
        window.draw(pauseOverlay);
        window.draw(textPausedTitle);
        btnResume.draw(window);
        btnBackToMenu.draw(window);
    }
    else if(!isGameOver)
    {
        window.draw(textQuestion);
        if (currentCategory == L"Chaos") 
        {
            window.draw(eventLabel);
        }
        window.draw(textTimer);
        window.draw(textCounter);
        btnAnswer0.draw(window,isFogActive,isTransitioning);
        btnAnswer1.draw(window,isFogActive,isTransitioning);
        btnAnswer2.draw(window,isFogActive,isTransitioning);
        btnAnswer3.draw(window,isFogActive,isTransitioning);  
    }
    else
    {
        window.draw(resultsPanel);
        window.draw(textGameOverTitle);
        window.draw(textRank);
        window.draw(textScore);
        window.draw(textPercentage);
        btnBackToMenu.draw(window);
        btnRestart.draw(window);
    }
}

void GamePlayScreen::colorizeButtons(int clickedIndex, int correctIndex)
{
    Button* buttons[4] = {&btnAnswer0,&btnAnswer1,&btnAnswer2,&btnAnswer3};
    for(int i=0;i<4;i++)
    {
        if(i==correctIndex)
        {
            buttons[i]->setBackgroundColor(sf::Color(0,160,0));
        }
        else if (i==clickedIndex && i != correctIndex)
        {
            buttons[i]->setBackgroundColor(sf::Color(100,0,0));
        }
        else
        {
            buttons[i]->setBackgroundColor(sf::Color(100,0,0));
        }
        
    }
}

int GamePlayScreen::handleInput(sf::RenderWindow&window, AudioManager&audio)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos); 

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if(isPaused)
        {
            if(btnResume.isClicked(mousePos))
            {
                audio.PlayClick();
                togglePause();
                return 0;
            }
            if(btnBackToMenu.isClicked(mousePos))
            {
                audio.PlayClick();
                return 1;
            }
        }
        else if(isGameOver)
        {
            if(btnBackToMenu.isClicked(mousePos))
            {
                audio.PlayClick();
                return 1;
            }
            if(btnRestart.isClicked(mousePos))
            {
                audio.PlayClick();
                return 3;
            }
        }
        else
        {
            if (isTransitioning) return 0;
            int clickedIndex = -1;

            if (btnAnswer0.isClicked(mousePos)) clickedIndex = 0;
            else if (btnAnswer1.isClicked(mousePos)) clickedIndex = 1;
            else if (btnAnswer2.isClicked(mousePos)) clickedIndex = 2;
            else if (btnAnswer3.isClicked(mousePos)) clickedIndex = 3;

            if (clickedIndex != -1)
            {
                audio.PlayClick();
                int correct = questions[currentQuestionIndex].correctIndex;
                
                if (clickedIndex == questions[currentQuestionIndex].correctIndex)
                {
                    score++;
                }

                isTransitioning = true;
                transitionClock.restart();

                colorizeButtons(clickedIndex,correct);
            }
        }
    }
    return 0;
}