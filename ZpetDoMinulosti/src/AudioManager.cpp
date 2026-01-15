#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager():ClickSound(clickBuffer)
{
    isMusicEnabled = true;
    isAudioAvailable = true; 
}

bool AudioManager::loadAssets()
{
    std::string pathPrefix = "assets/";


    if (!clickBuffer.loadFromFile(pathPrefix + "click.wav"))
    {
        if (!clickBuffer.loadFromFile("assets/click.wav"))
        {
            std::cerr << "CHYBA: click.wav se nepodarilo nacist (nebo chybí audio zarizeni)." << std::endl;
            isAudioAvailable = false;
        }
    }

    if (isAudioAvailable)
    {
        ClickSound.setBuffer(clickBuffer);
        ClickSound.setVolume(50.0f);
    }

    if (isAudioAvailable)
    {
        if (!backgroundMusic.openFromFile(pathPrefix + "music.ogg"))
        {
            if (!backgroundMusic.openFromFile("assets/music.ogg"))
            {
                std::cerr << "Varovani: music.ogg se nepodarilo nacist." << std::endl;
            }
        }
        
        backgroundMusic.setLoop(true); 
        backgroundMusic.setVolume(20.0f);
    }

    return true; 
}

void AudioManager::playMusic()
{
    if (!isAudioAvailable) return;

    if (isMusicEnabled && backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
    {
        backgroundMusic.play();
    }
}

void AudioManager::stopMusic()
{
    if (!isAudioAvailable) return;
    backgroundMusic.stop();
}

void AudioManager::toggleMusic()
{
    isMusicEnabled = !isMusicEnabled; 
    
    if (isMusicEnabled)
    {
        playMusic();
    }
    else
    {
        stopMusic();
    }
}

bool AudioManager::isMusicPlaying() const 
{
    if (!isAudioAvailable) return false;
    return backgroundMusic.getStatus() == sf::SoundSource::Status::Playing;
}

void AudioManager::PlayClick()
{
    // BEZPEČNOSTNÍ POJISTKA
    if (!isAudioAvailable) return;

    ClickSound.play();
}
