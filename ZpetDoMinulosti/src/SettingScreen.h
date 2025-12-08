#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"
#include "AudioManager.h" // Předpokládám, že tento soubor máte vytvořený

class SettingsScreen
{
public:
    // Konstruktor: Přijímá rozměry okna a font pro texty
    SettingsScreen(float width, float height, const sf::Font& font);

    // Vykreslení celé obrazovky nastavení
    void draw(sf::RenderWindow& window);

    // Zpracování vstupu (klikání myší)
    // Vrací 'true', pokud se má uživatel vrátit do menu (stisk tlačítka Zpět)
    bool handleInput(sf::RenderWindow& window, AudioManager& audio);

private:
    sf::Text titleText;       // Nadpis "NASTAVENÍ"

    Button btnToggle;         // Tlačítko pro vypnutí/zapnutí hudby
    Button btnResolution;     // Tlačítko pro změnu rozlišení
    Button btnBack;           // Tlačítko pro návrat zpět

    std::vector<sf::Vector2u> resolutions; // Seznam možných rozlišení (např. 800x600, 1920x1080)
    int currentResIndex;                   // Index aktuálně vybraného rozlišení v seznamu

    // Pomocná funkce: aktualizuje text na tlačítku rozlišení (např. "Rozlišení: 800x600")
    void updateResolutionButtonText();

    // Pomocná funkce: srovná tlačítka na střed podle nové šířky/výšky okna
    void recalculatePosition(float width, float height);
};