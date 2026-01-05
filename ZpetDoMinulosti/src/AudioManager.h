#pragma once
#include <SFML/Audio.hpp>

class AudioManager
{
public:

    AudioManager();   
    bool loadAssets(); 
    
    void playMusic(); 
    void stopMusic(); 
    void toggleMusic(); 
    bool isMusicPlaying() const; 

    void PlayClick(); 

private:
    sf::Music backgroundMusic; 
    sf::SoundBuffer clickBuffer; 
    sf::Sound ClickSound;

    bool isMusicEnabled; 
};