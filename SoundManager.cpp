#include "SoundManager.h"
#include <iostream>


SoundManager::SoundManager()
    : winSound(winBuffer),
      drawSound(drawBuffer)
{
    loadResources();
}

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::loadResources() {

    if (!draftMusic.openFromFile("images/sounds/draft_bg.ogg")) {
        std::cerr << "Failed to load draft_bg.mp3" << std::endl;
    }


    draftMusic.setLooping(true);
    draftMusic.setVolume(30.0f);

    if (!matchMusic.openFromFile("images/sounds/ucl_anthem.ogg")) {
        std::cerr << "Failed to load ucl_anthem.mp3" << std::endl;
    }
    matchMusic.setVolume(50.0f);

    if (winBuffer.loadFromFile("images/sounds/becali_win.wav")) {
        winSound.setVolume(100.0f);
    }

    if (drawBuffer.loadFromFile("images/sounds/becali_lose.wav")) {
        drawSound.setVolume(100.0f);
    }
}

void SoundManager::playDraftMusic() {

    if (draftMusic.getStatus() != sf::SoundSource::Status::Playing) {
        draftMusic.play();
    }
}

void SoundManager::stopDraftMusic() {
    draftMusic.stop();
}

void SoundManager::playMatchMusic() {
    matchMusic.play();
}

void SoundManager::stopMatchMusic() {
    matchMusic.stop();
}

void SoundManager::playWinSound() {
    winSound.play();
}

void SoundManager::playDrawSound() {
    drawSound.play();
}
void SoundManager::stopEffects() {
    if (winSound.getStatus() == sf::SoundSource::Status::Playing) {
        winSound.stop();
    }
    if (drawSound.getStatus() == sf::SoundSource::Status::Playing) {
        drawSound.stop();
    }
}