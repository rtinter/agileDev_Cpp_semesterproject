#include "Conclusions.hpp"
#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace game {

    // Constructor: Initializes the game with name, description, rules, controls, and loads questions
    Conclusions::Conclusions() :
    abstract_game::Game(abstract_game::GameID::ABSURD_QUESTIONS),
    _selectedOption{std::nullopt},
    _showCorrectMessage{false},
    _solved{0}
    {
        _gameName = "Absurde Fragen";
        _gameDescription = "Beantworte die absurden Fragen.";
        _gameRules = "1. Lies die angezeigte Frage.\n"
                     "2. Entscheide, ob die Schlussfolgerung richtig oder falsch ist.\n"
                     "3. Bestätige deine Auswahl mit dem Bestätigen Button.\n";
        _gameControls = "Linke Maustaste zur Auswahl der Antwort und ebenfalls zum Bestätigen des Buttons";

        loadQuestions();
    }

    // Renders the game, including start box, end box, and the game itself
    void Conclusions::render() {
        ui_elements::InfoBox(
                _gameID,
                _showStartBox,
                "Startbox",
                _gameName,
                _gameDescription,
                _gameRules,
                _gameControls,
                [this] {
                    start();
                }
        ).render();

        ui_elements::InfoBox(
                _gameID,
                _showEndBox,
                "Endbox",
                _endBoxTitle,
                _endBoxText,
                [this] {
                    reset();
                }
        ).render();

        if (_isGameRunning) {
            renderGame();
        }
    }

    // Renders the main game content
    void Conclusions::renderGame() {
        ui_elements::Window("Absurde Fragen").render([this]() {
            if (_questions.empty()) {
                std::cout << "No questions loaded." << std::endl;
                return;
            }

            if (_showCorrectMessage) {
                renderCorrectMessage();
            } else {
                renderQuestion();
            }
        });
    }

    // Renders the current question and answer options
    void Conclusions::renderQuestion() {
        float const buttonWidth {100.0f};
        float const buttonOffsetX {(ImGui::GetWindowWidth() - buttonWidth) / 2.0f};
        float const itemWidth {100.0f};
        float const itemOffsetX {(ImGui::GetWindowWidth() - itemWidth) / 2.0f};
        float const textWrapWidth {1350.0f}; // Set a fixed wrap width for the question text

        ImGui::PushFont(commons::Fonts::_header2);

        // Calculate the position to center the text within textWrapWidth
        float textStartPosX {(ImGui::GetWindowWidth() - textWrapWidth) / 2.0f};
        ImGui::SetCursorPosX(textStartPosX);

        // Wrap the text within the specified width and center it
        ImGui::PushTextWrapPos(textStartPosX + textWrapWidth);
        ImGui::TextWrapped(_currentQuestion.questionText.c_str());
        ImGui::PopTextWrapPos();

        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Spacing();

        // Centering and rendering the radio buttons
        ImGui::SetCursorPosX(itemOffsetX);
        if (ImGui::RadioButton("Stimmt", _selectedOption.has_value() && _selectedOption.value())) {
            _selectedOption = true;
        }

        ImGui::SetCursorPosX(itemOffsetX);
        if (ImGui::RadioButton("Stimmt nicht", _selectedOption.has_value() && !_selectedOption.value())) {
            _selectedOption = false;
        }

        ImGui::Spacing();

        // Centering and rendering the confirmation button
        ImGui::SetCursorPosX(buttonOffsetX);
        if (ImGui::Button("Bestätigen")) {
            checkAnswer(_selectedOption);
            _selectedOption = std::nullopt;
        }
    }

    // Renders a message and current score when the correct answer is given
    void Conclusions::renderCorrectMessage(){
        auto now {std::chrono::steady_clock::now()};
        auto duration { std::chrono::duration_cast<std::chrono::seconds>(now - _correctMessageStartTime).count()};

        if (duration < 2) {
            ImGui::PushFont(commons::Fonts::_header2);
            ImGui::Spacing();
            ImGui::Spacing();
            ui_elements::TextCentered("Richtige Antwort!");
            ImGui::PopFont();
            _solvedText = "Du hast bisher " + std::to_string(_solved) + " Fragen richtig beantwortet";
            ui_elements::TextCentered(_solvedText.c_str());
            ImGui::Spacing();
            ui_elements::TextCentered(" Weiter so!");
        } else {
            _showCorrectMessage = false;
            generateRandomQuestion();
        }
    }

    // Renders the game over message in the endBox when an incorrect answer is given
    void Conclusions::renderGameOver() {
        stop();

        _endBoxTitle = "Das war leider nicht korrekt";
        _endBoxText = "Du hast diesmal " +
                      std::to_string(_solved) + " in Folge lösen können";
    }

    // Loads questions/answers from the conclusions JSON file relative to bin dir
    void Conclusions::loadQuestions() {
        logger::Logger& logger { logger::Logger::getInstance()};

        std::fstream file;
        try {
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            file.open("./config/games/conclusions.json");
            logger.log("Loading files for Conclusions-Game", QueueEntryType::INFORMATION);

            json data;
            file >> data;
            file.close();

            for (const auto &elem : data) {
                Question q;
                q.questionText = elem["frage"];
                q.isCorrectAnswer = elem["antwort"].get<int>() == 1;
                _questions.emplace_back(q);
            }

            // Log the loaded questions
            for (const auto &q : _questions) {
                logger.log("Loaded question: " + q.questionText + " Answer: " + std::to_string(q.isCorrectAnswer), QueueEntryType::INFORMATION);
            }

        } catch (std::exception const &e) {
            std::stringstream error;
            error << "Error opening or reading the file conclusions.json: " << e.what();
            std::cerr << error.str() << std::endl;
            logger.log(error.str(), QueueEntryType::SEVERE);
        }
    }

    // Generates a random question from the loaded questions
    void Conclusions::generateRandomQuestion() {
        if (_questions.empty()) {
            return;
        }

        Question newQuestion;
        do {
            newQuestion = commons::RandomPicker::pickRandomElement(_questions);
        } while (_lastQuestion.has_value() && newQuestion.questionText == _lastQuestion->questionText);

        _currentQuestion = newQuestion;
        _lastQuestion = _currentQuestion;
    }


    // Checks the selected answer and updates the game state
    void Conclusions::checkAnswer(std::optional<bool> &selectedOption){
        if (selectedOption.has_value() && selectedOption.value() == _currentQuestion.isCorrectAnswer) {
            _solved++;
            _showCorrectMessage = true;
            _correctMessageStartTime = std::chrono::steady_clock::now();
        } else {
            renderGameOver();
        }
    }


    // Starts the game by resetting counters and generating the first question
    void Conclusions::start() {
        _solved = 0;
        generateRandomQuestion();
        _isGameRunning = true;
        _showEndBox = false;
        _showStartBox = false;
        _selectedOption = std::nullopt;
    }

    void Conclusions::stop() {
        updateStatistics();
        _isGameRunning = false;
        _showEndBox = true;
    }

    void Conclusions::reset() {
        start();
    }

    // Updates game statistics with the number of correct answers
    void Conclusions::updateStatistics() {
        abstract_game::GameSessionManager::getCurrentSession()->addNewGameRunThrough("korrekte Antworten", _solved);
    }

    std::string Conclusions::getName() const {
        return Game::getName();
    }
} // namespace game
