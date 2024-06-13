#ifndef ATHENA_SEQUENCE_H
#define ATHENA_SEQUENCE_H

#include <array>
#include "Game.hpp"

namespace sequence {

    class Sequence : public abstract_game::Game {

        std::string _endBoxString;
        int _longestReproducedSequence;

        static int constexpr _NUMBER_OF_BUTTONS {9};

        enum GameMode {
            WATCH,  //watch the sequence and try to remember it
            REPEAT  //repeat the sequence you just saw
        };
        GameMode _currentGameMode;

        int _levelCounter;

        std::chrono::steady_clock::time_point _startHighlightingHere;
        std::chrono::steady_clock::time_point _stopHighlightingHere;
        int _lightUpDurationInSeconds {1};
        //bool _isLitUp {false};
        bool _isLastButtonOfSequence {false};
        bool _sequenceShowMode {false};
        bool _canShowNextButtonInSequence{false};

        std::vector<int> _buttonsClickedSequence;
        //true -> Button lights up
        std::vector<bool> _buttonStates;
        std::array<int, _NUMBER_OF_BUTTONS> _buttonStatess;



        void displayButtons();

        void isClickedInCorrectOrder();

        void chooseNextRandomButton();

        void showSequence();

        void lightUp(int &buttonState);

        void checkLitUpExpired(int &buttonState);

    public:
        Sequence();

        void render() override;

        void renderGame() override;

        void start() override;

        void reset() override;

        void stop() override;

        void updateStatistics() override;

    };

} // sequence

#endif //ATHENA_SEQUENCE_H
