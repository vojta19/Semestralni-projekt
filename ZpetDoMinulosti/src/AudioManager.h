#pragma once
#include <SFML/Audio.hpp>

class AudioManager
{
public:

    AudioManager();   //konstruktor, nastaví výchozí hodnoty
    bool loadAssets(); //funkce pro načtení souborů z disku, zobrazí true když se povede
    
    //ovládání hudby
    void playMusic(); //spustí hudbu
    void stopMusic(); //zastaví hudbu
    void toggleMusic(); //přepínač, když hraje -> vypne, když nehraje -> zapne
    bool isMusicPlaying() const; //zjistí stav, pro tlačítko v nastavení

    //zvukové efekty
    void PlayClick(); //přehraje krátké cvaknutí

private:
    sf::Music backgroundMusic; //hudba na pozadí
    sf::SoundBuffer clickBuffer; //zvuk kliknutí (data v paměti + přehrávač)
    sf::Sound ClickSound;

    bool isMusicEnabled; //pomocná proměna, abychom věděli jestli uživatel chce hudbu
};