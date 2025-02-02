#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Game.hpp"
#include "QuestionBank.hpp"
#include "Timer.hpp"

namespace games {
    /**
     * @brief Class representing the Remembering game.
     */
    class Remembering final : public abstract_game::Game {
    public:
        /**
         * @brief Constructor: Initializes the Remembering game.
         */
        explicit Remembering();

        /**
         * @brief Renders the game UI elements.
         */
        void render() override;

        /**
         * @brief Renders the main game content.
         */
        void renderGame() override;

        /**
         * @brief Starts the game.
         */
        void start() override;

        /**
         * @brief Stops the game.
         */
        void stop() override;

        /**
         * @brief Resets the game to its initial state.
         */
        void reset() override;

        /**
         * @brief Updates the game statistics.
         */
        void updateStatistics() override;

    private:
        std::string _userTextBuffer;
        bool _showText{true};
        bool _showContinueButton{false};
        bool _submitted{false};
        int _score{0};
        ui_elements::Timer _timer{"Remembering Game", 40};
        static std::vector<int> _selectedAnswers;

        std::unique_ptr<remembering::QuestionBank> _questionBank;
        remembering::QuestionBank::QuestionSet _currentQuestionSet;

        /**
        * @brief Displays centered text within the game window.
        * @param text The text to be displayed.
        */
        static void displayCenteredText(std::string const &text);

        /**
         * @brief Selects a random set of questions from the question bank.
         */
        void selectRandomQuestionSet();

        /**
         * @brief Displays the evaluation of the player's performance.
         * @param score The score achieved by the player.
         * @param size The total number of questions.
         * @return A string representing the evaluation.
         */
        static std::string displayEvaluation(int const &score, int const &size);

        /**
         * @brief Renders a question and its possible answers.
         * @param index The index of the question.
         * @param q The question to be rendered.
         * @param selectedAnswer The index of the selected answer.
         */
        void renderQuestion(int const &index, remembering::QuestionBank::Question const &q, int &selectedAnswer) const;

        /**
         * @brief Sets the styles for the UI elements.
         */
        static void setStyles();

        /**
         * @brief Renders the submit buttons for the game.
         */
        void renderSubmitButtons();
    };
} // games
