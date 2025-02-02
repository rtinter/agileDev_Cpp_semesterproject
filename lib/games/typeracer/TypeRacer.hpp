#pragma once

#include <chrono>
#include <Colors.hpp>
#include <set>
#include <imgui.h>

#include "Game.hpp"

namespace games {
    /**
    *Type Racer Game
    * Type Racer gives the user two sentences and measures the typing speed,
    * as well as showing correct and incorrect letters.
    */
    class TypeRacer final : public abstract_game::Game {
        ImVec4 _windowColor{commons::Colors::LIGHT_GRAY};
        float _windowWidth;
        float _textWidth;
        std::string _sentence;
        std::set<int> _mistypedIndices;
        std::chrono::steady_clock::time_point _startPoint;
        bool _runTimer{false};
        float _wpm{0.0f};
        char _input[256];
        int _randomIndex{0};

    public:
        explicit TypeRacer();

        void render() override;

        void renderGame() override;

        void start() override;

        void stop() override;

        void reset() override;

        void updateStatistics() override;

        std::string getName() const override;

        ~TypeRacer() override;
    };
} // games
