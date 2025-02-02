#pragma once

#include <array>
#include <random>
#include <string>

#include "Colors.hpp"
#include "Game.hpp"
#include "Timer.hpp"

namespace games {
    /**
     * @brief: provides logic and visuals for the game 'Farbe & Wort'
     */
    class ColorMatch final : public abstract_game::Game {
        static constexpr int NUMBER_OF_RANDOM_COLORS{3};
        bool _isTimeForNewRandomColors;
        int _indexOfCurrentColor;
        int _numberOfCorrectClicksInTotal;
        int _numberOfCorrectClicksSinceLastError;
        int _longestStreak;

        enum GameMode {
            MATCH_IMVEC4, // click text button according to font color
            MATCH_STRING // click color button according to word
        };

        GameMode _currentGameMode;
        ui_elements::Timer _timer{"Color Match Game", 60};

        std::vector<std::string> const _availableColorsString{"rot", "blau", "gelb", "lila", "grün"};
        std::vector<ImVec4> const _availableColorsImVec4{
            commons::Colors::RED, commons::Colors::BLUE,
            commons::Colors::YELLOW, commons::Colors::PURPLE,
            commons::Colors::GREEN
        };
        std::vector<std::string> _randomColorsString;
        std::vector<ImVec4> _randomColorsImVec4;
        std::array<std::string, NUMBER_OF_RANDOM_COLORS> const _clickedColors;

        void displayRandomColors() const;

        void pickRandomColorsString();

        void pickRandomColorsImVec4();

        void displayAnswerButtons();

        void onClick(bool const &isCurrentColor);

    public:
        ColorMatch();

        void render() override;

        void renderGame() override;

        void start() override;

        void reset() override;

        void updateStatistics() override;

        void stop() override;
    };
} // games
