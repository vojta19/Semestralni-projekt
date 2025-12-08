#include "SettingScreen.h"
#include <string> 

SettingsScreen::SettingsScreen(float width, float height, const sf::Font& font)
    : titleText(font),
      btnToggle(0, 0, 300, 50, L"Hudba ZAP/VYP", font),
      btnResolution(0, 0, 300, 50, L"Rozlišení: 800x600", font),
      btnBack(0, 0, 150, 40, L"Zpět", font)
{
    // --- Nadpis ---
    titleText.setString(L"NASTAVENÍ");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin({
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f
    });

    
    resolutions.push_back({800, 600});
    resolutions.push_back({1280, 720});
    resolutions.push_back({1366, 768});
    resolutions.push_back({1920, 1080});
    resolutions.push_back({2560, 1440});

    currentResIndex = 0;
    updateResolutionButtonText();
    recalculatePosition(width, height);
}

void SettingsScreen::recalculatePosition(float width, float height)
{
    
    titleText.setPosition({width / 2.0f, 100.0f});

    float centerX = width / 2.0f - 150.0f;

    btnToggle.setPosition(centerX, 200.0f);
    btnResolution.setPosition(centerX, 300.0f);

    btnBack.setPosition(width - 170.0f, height - 60.0f);
}

void SettingsScreen::updateResolutionButtonText()
{
    sf::Vector2u res = resolutions[currentResIndex];
    
    // Čeština
    std::wstring text = L"Rozlišení: " + std::to_wstring(res.x) + L"x" + std::to_wstring(res.y);
    
    btnResolution.setText(text);
}

void SettingsScreen::draw(sf::RenderWindow& window)
{
    window.draw(titleText);
    btnBack.draw(window);
    btnResolution.draw(window);
    btnToggle.draw(window);
}

bool SettingsScreen::handleInput(sf::RenderWindow& window, AudioManager& audio)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        if (btnBack.isClicked(mousePos))
        {
            audio.PlayClick();
            return true; 
        }

        if (btnToggle.isClicked(mousePos))
        {
            audio.PlayClick();
            audio.toggleMusic();
            sf::sleep(sf::milliseconds(200)); 
        }

        if (btnResolution.isClicked(mousePos))  
        {
            audio.PlayClick();

            currentResIndex++;
            if (currentResIndex >= resolutions.size())
            {
                currentResIndex = 0;
            }

            sf::Vector2u newRes = resolutions[currentResIndex];

            sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

            // Pokud se vybrané rozlišení rovná rozlišení monitoru -> Fullscreen
            if (newRes.x == desktopMode.size.x && newRes.y == desktopMode.size.y)
            {
            // Režim Fullscreen (bez rámečků, exkluzivní režim)
            window.create(sf::VideoMode(newRes), L"Zpět do minulosti", sf::Style::None, sf::State::Fullscreen);
            }
            else
            {
            // Režim Okno (s křížkem a lištou)
            window.create(sf::VideoMode(newRes), L"Zpět do minulosti", sf::Style::Default, sf::State::Windowed);
            }

            window.setFramerateLimit(60);

            sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(newRes.x), static_cast<float>(newRes.y)}));
            window.setView(view);

            updateResolutionButtonText();
            recalculatePosition(static_cast<float>(newRes.x), static_cast<float>(newRes.y));
            
        }   
    return false;
}