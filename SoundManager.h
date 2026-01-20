#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <memory>

class SoundManager {
    sf::Music draftMusic;
    sf::Music matchMusic;

    sf::SoundBuffer winBuffer;
    sf::Sound winSound;

    sf::SoundBuffer drawBuffer;
    sf::Sound drawSound;

    SoundManager();

public:
    static SoundManager& getInstance();

    SoundManager(const SoundManager&) = delete;
    void operator=(const SoundManager&) = delete;

    void loadResources();

    void playDraftMusic();
    void stopDraftMusic();

    void playMatchMusic();
    void stopMatchMusic();

    void playWinSound();
    void playDrawSound();
    void stopEffects();
};