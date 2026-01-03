#include "AudioManager.h"
#include <iostream>

//AudioManager::AudioManager() : ClickSound(clickBuffer)
//{
  //  isMusicEnabled=true;  //konstruktor, na začátku chceme aby byla hudba povolená
//}

//bool AudioManager::loadAssets()
//{
//    if(!backgroundMusic.openFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/music.ogg")) //primární cesta načtení souboru
//    {
//        if(!backgroundMusic.openFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/music.ogg")) //záložní cesta načtení souboru
//        {
//            std::cerr << "CHYBA: Nepodarilo se nacist music.ogg!" << std::endl; //neúspěch tolik nevadí, hru to nezastaví
//        }
//    }

//    backgroundMusic.setLooping(true); //ať hraje pořád do kola
//    backgroundMusic.setVolume(20.f); //hlasitost na 20%

//    if(!clickBuffer.loadFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/click.wav")) //načtení zvuku cvaknutí
//    {
//        if(!clickBuffer.loadFromFile("/workspaces/Semestralni-projekt/ZpetDoMinulosti/assets/click.wav"))
//        {
//            std::cerr << "CHYBA: Nepodarilo se nacist click.wav!" << std::endl;
//            return false; //tady vrátíme false, je něco špatně
//        }
//    }
    
//    ClickSound.setBuffer(clickBuffer); //propojíme data (buffer) s přehrávačem (sound)
//    return true; //vše ok
//}

//void AudioManager::playMusic()
//{
//    if(isMusicEnabled && backgroundMusic.getStatus() !=sf::SoundSource::Status::Playing) //hrajeme jenom když je to povolené a zovna to nehraje
//    {
//        backgroundMusic.play();
//    }
//}

//void AudioManager::stopMusic()
//{
//    backgroundMusic.stop();
//}

//void AudioManager::toggleMusic() //logika pro tlačítko v nastavení
//{
//    isMusicEnabled=!isMusicEnabled; //otočíme hodnotu, true -> false a naopak
    
//    if(isMusicEnabled)
//    {
//        playMusic();
//    }
//    else
//    {
//        stopMusic();
//    }
//}

//bool AudioManager::isMusicPlaying() const 
//{
//    return isMusicEnabled;
//}

//void AudioManager::PlayClick()
//{
//    ClickSound.play();
//}

