#include "SettingScreen.h"
#include <string> 

SettingsScreen::SettingsScreen(float width, float height, const sf::Font& font)
    : titleText(),
      btnToggle(0, 0, 300, 50, L"Hudba ZAP/VYP", font),
      btnResolution(0, 0, 300, 50, L"Rozlišení: 800x600", font),
      btnBack(0, 0, 150, 40, L"Zpět", font)
{
    titleText.setFont(font);
    titleText.setString(L"NASTAVENÍ");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin({
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
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
    float centerX = width / 2.0f;
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin({
        titleRect.left + titleRect.width / 2.0f,
        titleRect.top + titleRect.height / 2.0f
        });
    titleText.setPosition({ centerX, height * 0.22f });

    float buttonHalfWidth = 150.0f;
    float btnX = centerX - buttonHalfWidth;
    float groupCenterY = height * 0.55f;
    float offset = 45.0f;

    btnToggle.setPosition(btnX, groupCenterY - offset);
    btnResolution.setPosition(btnX, groupCenterY + offset);
    btnBack.setPosition(width - 170.0f, height - 60.0f);
}


void SettingsScreen::updateResolutionButtonText()
{
    sf::Vector2u res = resolutions[currentResIndex];
    
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

            sf::VideoMode mode(newRes.x,newRes.y);
            if (newRes.x == desktopMode.width && newRes.y == desktopMode.height)
            {
            window.create(mode, L"Zpět do minulosti",sf::Style::Fullscreen);
            }
            else
            {
            window.create(mode, L"Zpět do minulosti",sf::Style::Default);
            }

            window.setFramerateLimit(60);

            sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(newRes.x), static_cast<float>(newRes.y)}));
            window.setView(view);

            updateResolutionButtonText();
            recalculatePosition(static_cast<float>(newRes.x), static_cast<float>(newRes.y));
            
        }   
    return false;
}