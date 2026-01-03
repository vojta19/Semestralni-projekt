#include "GamePlayScreen.h"
#include <iostream>
#include <cmath>
#include "QuestionManager.h"

GamePlayScreen::GamePlayScreen(float width, float height, const sf::Font&font)
:font(font), windowHeight(height), windowWidth(width),

textQuestion(),
textTimer(),
textCounter(),
textGameOverTitle(),
textScore(),
textPercentage(),
//textThanks(),
textRank(),
textPausedTitle(),

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

  //  textThanks.setFont(font);
 //   textThanks.setCharacterSize(30);
 //   textThanks.setString(L"Děkujeme za hru! ");
 //   textThanks.setFillColor(sf::Color::Yellow);

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

    isGameOver = false;
    isPaused = false;
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
    score = 0;
    currentQuestionIndex = 0;

    setTimeForDifficulty(difficulty);
    loadQuestions(category, difficulty);
    loadNextQuestionUI();
}

void GamePlayScreen::loadQuestions(std::wstring category, std::wstring difficulty)
{
    questions.clear();

    // 1. Zkusíme stáhnout a přeložit otázky (to může chvíli trvat!)
    std::cout << "Stahuji a prekladam otazky... Cekejte prosim." << std::endl;
    std::vector<Question> downloadedQuestions = QuestionManager::fetchQuestions(category, difficulty);

    // 2. Pokud se stažení povedlo (máme aspoň 1 otázku)
    if (!downloadedQuestions.empty())
    {
        questions = downloadedQuestions;
        std::cout << "Uspesne stazeno " << questions.size() << " otazek." << std::endl;
    }
    else
    {
        // 3. FALLBACK - Pokud selže internet, vygenerujeme nouzové otázky
        std::cerr << "Chyba stahovani! Pouzivam zalozni otazky." << std::endl;
        
        for (int i = 0; i < 5; i++)
        {
            Question q;
            q.text = L"Chyba připojení k internetu. (Záložní otázka " + std::to_wstring(i + 1) + L")";
            q.answers = {L"Odpověď A", L"Odpověď B", L"Odpověď C", L"Odpověď D"};
            q.correctIndex = 0;
            questions.push_back(q);
        }
    }
}

void GamePlayScreen::loadNextQuestionUI()
{
    if (currentQuestionIndex >= questions.size()) {
        finishGame();
        return;
    }

    remainingTime = timeLimit;
    Question& q = questions[currentQuestionIndex];

    // --- ZALAMOVÁNÍ HLAVNÍ OTÁZKY ---
    std::wstring rawText = q.text;
    std::wstring wrappedText;
    std::wstring line;
    
    // Nastavíme font a velikost, abychom mohli měřit
    textQuestion.setString(rawText);
    textQuestion.setCharacterSize(24); // Velikost písma otázky
    
    // Maximální šířka textu (např. 80% šířky okna)
    float maxWidth = windowWidth * 0.8f;

    // Rozborka textu na slova
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
        
        if (textQuestion.getLocalBounds().width > maxWidth)
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

    // Nastavíme finální zalomený text
    textQuestion.setString(wrappedText);
    
    // Vycentrování
    sf::FloatRect qRect = textQuestion.getLocalBounds();
    textQuestion.setOrigin({
        qRect.left + qRect.width / 2.0f,
        qRect.top + qRect.height / 2.0f
    });
    
    // Pozice: střed šířky, a trochu níž od vrchu (aby se vešly i 3 řádky)
    textQuestion.setPosition({windowWidth / 2.0f, 150.0f});

    // --- NASTAVENÍ ODPOVĚDÍ (Zbytek funkce beze změny) ---
    btnAnswer0.setText(q.answers[0]);
    btnAnswer1.setText(q.answers[1]);
    btnAnswer2.setText(q.answers[2]);
    btnAnswer3.setText(q.answers[3]);

    textCounter.setString(std::to_wstring(currentQuestionIndex + 1) + L" / " + std::to_wstring(questions.size()));
}


void GamePlayScreen::update(sf::Time deltaTime)
{
    if(isGameOver || isPaused) return;
    remainingTime -= deltaTime.asSeconds();

    int seconds = static_cast<int>(remainingTime);
    int decis = static_cast<int>((remainingTime - seconds) * 10); 
    if (remainingTime < 0) remainingTime = 0; 
    textTimer.setString(std::to_wstring(seconds) + L"." + std::to_wstring(decis) + L" s"); 
    
    if (remainingTime < 5.0f) 
    { 
        blinkTimer += deltaTime.asSeconds(); 
        if (blinkTimer >= 0.2f) 
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
    if (remainingTime <= 0.0f) 
    { 
        currentQuestionIndex++; loadNextQuestionUI(); 
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

    float centerX = width/2.0f;
    float centerY = height / 2.0f;

    if(isPaused)
    {
        pauseOverlay.setSize({width,height});
        pauseOverlay.setPosition({0,0});

        sf::FloatRect pRect = textPausedTitle.getLocalBounds();
        textPausedTitle.setOrigin({pRect.left + pRect.width / 2.0f, pRect.top + pRect.height / 2.0f});
        textPausedTitle.setPosition({centerX, height * 0.25f});

        btnResume.setPosition(centerX - 150.0f, centerY);
        btnBackToMenu.setPosition(centerX - 150.0f, centerY + 80.0f);
    }
    else if(!isGameOver && !isPaused)
    {
        sf::FloatRect qRect = textQuestion.getLocalBounds();
        textQuestion.setOrigin({qRect.left + qRect.width / 2.0f, qRect.top + qRect.height / 2.0f});
        textQuestion.setPosition({width / 2.0f,150.0f});

        sf::FloatRect tRect = textTimer.getLocalBounds();
        textTimer.setOrigin({tRect.left + tRect.width, 0.0f});
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
        float centerX = width/2.0f;
        float centerY = height/2.0f;

        resultsPanel.setSize({800.0f,450.0f});
        resultsPanel.setOrigin({400.0f,225.0f});
        resultsPanel.setPosition({centerX,centerY});

        sf::FloatRect titleRect = textGameOverTitle.getLocalBounds();
        textGameOverTitle.setOrigin({titleRect.left + titleRect.width / 2.0f, 0});
        textGameOverTitle.setPosition({centerX,centerY - 180.0f});

        sf::FloatRect rankRect = textRank.getLocalBounds();
        textRank.setOrigin({rankRect.left + rankRect.width / 2.0f,0});
        textRank.setPosition({centerX,centerY - 100.0f});

        sf::FloatRect scoreRect = textScore.getLocalBounds();
        textScore.setOrigin({scoreRect.left + scoreRect.width / 2.0f, 0});
        textScore.setPosition({centerX,centerY - 20.0f});

        sf::FloatRect percRect = textPercentage.getLocalBounds();
        textPercentage.setOrigin({percRect.left + percRect.width / 2.0f, 0});
        textPercentage.setPosition({centerX,centerY + 30.0f});

       // sf::FloatRect thanksRect = textThanks.getLocalBounds();
      //  textThanks.setOrigin({thanksRect.position.x + thanksRect.size.x / 2.0f, 0});
      //  textThanks.setPosition({width / 2.0f, 400.0f});

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
        window.draw(textTimer);
        window.draw(textCounter);
        btnAnswer0.draw(window);
        btnAnswer1.draw(window);
        btnAnswer2.draw(window);
        btnAnswer3.draw(window);  
    }
    else
    {
        window.draw(resultsPanel);
        window.draw(textGameOverTitle);
        window.draw(textRank);
        window.draw(textScore);
        window.draw(textPercentage);
        //window.draw(textThanks);
        btnBackToMenu.draw(window);
        btnRestart.draw(window);
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
            int clickedIndex = -1;

            if (btnAnswer0.isClicked(mousePos)) clickedIndex = 0;
            else if (btnAnswer1.isClicked(mousePos)) clickedIndex = 1;
            else if (btnAnswer2.isClicked(mousePos)) clickedIndex = 2;
            else if (btnAnswer3.isClicked(mousePos)) clickedIndex = 3;

            if (clickedIndex != -1)
            {
                audio.PlayClick();
                
                if (clickedIndex == questions[currentQuestionIndex].correctIndex)
                {
                    score++;
                }
               currentQuestionIndex++;
                loadNextQuestionUI();
            }
        }
    }
    return 0;
}