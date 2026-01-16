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
        playMenuMusic();
    }
    return true;

}

void AudioManager::switchTrack(const std::string& filename)
{
    if (!isAudioAvailable) return;
	backgroundMusic.stop();

	std::string fullPath = "assets/" + filename;
    if (!backgroundMusic.openFromFile(fullPath))
    {
        std::cerr << "CHYBA: Hudbu se nepodarilo nacist: " << fullPath << std::endl;
        return;
	}
	backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(20.0f);

    if (isMusicEnabled)
    {
      backgroundMusic.play();
    }
}

void AudioManager::playMenuMusic()
{
    switchTrack("music_menu.ogg");
}

void AudioManager::playGameMusic(std::wstring category)
{
    std::string filename;

    if (category == L"Starověk")
    {
        filename = "music_ancient.ogg";
    }
    else if (category == L"Středověk")
    {
        filename = "music_medieval.ogg";
    }
    else if (category == L"Moderní dějiny")
    {
        filename = "music_modern.ogg";
    }
    else if (category == L"Chaos")
    {
        filename = "music_chaos.ogg";
    }
    else
    {
        filename = "music_menu.ogg";
    }
    switchTrack(filename);
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
        if (backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
            backgroundMusic.play();
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
    if (!isAudioAvailable) return;
    if (ClickSound.getBuffer() != nullptr)
    {
      ClickSound.play();
    }
}

