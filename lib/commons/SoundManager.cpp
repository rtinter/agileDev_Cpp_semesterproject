#include <iostream>
#include "SoundManager.hpp"
#include "../logger/Logger.hpp"
#include <SFML/Audio.hpp>

namespace commons {

    std::string const SoundManager::PATH = "./sounds/";

    sf::Sound SoundManager::_activeSound;

    std::unordered_map<commons::Sound, sf::SoundBuffer> SoundManager::_soundBufferMap;

    void SoundManager::loadSounds() {

        logger::Logger& instance {logger::Logger::getInstance()};
        // load sound buffers, add new sounds here
        initSoundBuffer(Sound::CLICK, "click.mp3");
        initSoundBuffer(Sound::LASER_GUN, "laser-gun.mp3");
        initSoundBuffer(Sound::EXPLOSION, "explosion.mp3");
        initSoundBuffer(Sound::ERROR, "error.mp3");
        initSoundBuffer(Sound::COMPLETE, "complete.mp3");
        initSoundBuffer(Sound::CORRECT, "correct.mp3");
        initSoundBuffer(Sound::BEEP, "beep.mp3");
        initSoundBuffer(Sound::BEEP_FAIL, "beep-fail.mp3");
        initSoundBuffer(Sound::CARD_FLIP, "card-flip.mp3");

        instance << "Sound Manager loaded";
    }

    void SoundManager::initSoundBuffer(commons::Sound sound, std::string const &filename) {

        sf::SoundBuffer soundBuffer;
        if (soundBuffer.loadFromFile(PATH + filename)) {
            _soundBufferMap[sound] = soundBuffer;
        } else {
            std::cout << "Error loading sound " << filename << std::endl;
        }

    }

    void SoundManager::playSound(commons::Sound sound) {
        playSound(sound, 100.0f);
    }

    void SoundManager::playSound(commons::Sound sound, int volumeInPercent) {
        playSound(sound, volumeInPercent, 1.0f);
        logger::Logger& instance {logger::Logger::getInstance()};
        instance << "Playing Sound";
    }

    void SoundManager::playSound(commons::Sound sound, int volumeInPercent, float pitch) {


#if (!defined(_WIN32))
        return;
#endif

        // check if sound manager is initialized
        if (_soundBufferMap.empty()) {
            std::cout << "SoundManager not initialized" << std::endl;
            return;
        }

        // check if sound is in map
        if (!_soundBufferMap.count(sound)) {
            std::cout << "Sound could not be loaded" << std::endl;
            return;
        }

        // play sound
        _activeSound.setBuffer(_soundBufferMap.at(sound));
        _activeSound.setVolume(volumeInPercent);
        _activeSound.setPitch(pitch);
        _activeSound.play();

        logger::Logger& instance {logger::Logger::getInstance()};
        instance << "Playing Sound";
    }
}
