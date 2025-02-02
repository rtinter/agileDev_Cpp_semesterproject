#include "SimonSays.hpp"

#include <random>
#include <thread>

#include "Centered.hpp"
#include "Colors.hpp"
#include "ColorTheme.hpp"
#include "Fonts.hpp"
#include "InfoBox.hpp"
#include "SoundPolice.hpp"
#include "Window.hpp"

namespace games {
    SimonSays::SimonSays() : Game(abstract_game::GameID::SEQUENCE), _currentGameMode(), _levelCounter(0),
                             _sequenceButtonIterator(0),
                             _correctClicksOfCurrentSequence(0),
                             _buttonStates() {
        _gameName = "Abfolge-Merken-Spiel";
        _gameDescription =
                "Unser Abfolge-Merken-Spiel soll die Fähigkeit testen, sich Abfolgen einzuprägen\nund korrekt wiederzugeben.\n";
        _gameRules = "Auf dem Bildschirm werden 9 verschiedene Buttons angezeigt.\n"
                "Diese Buttons werden nacheinander aufleuchten und anschließend müssen die Buttons in exakt dieser\n"
                "Reihenfolge angeklickt werden. Die Abfolge muss also wiederholt werden.\n"
                "Die Abfolge wird schrittweise ausgegeben. Also erst nur ein Button, dann zwei, dann drei usw.\n"
                "Teste deine Fähigkeiten und schaue, wie weit du kommst!\n";
        _gameControls = "Linke Maustaste: Klicken der Buttons in der korrekten Reihenfolge.\n";
    }

    void SimonSays::render() {
        ui_elements::InfoBox(_gameID, _showStartBox, "Startbox", _gameName, _gameDescription, _gameRules, _gameControls,
                             [this] {
                                 start();
                             }).render();

        ui_elements::InfoBox(_gameID, _showEndBox, "Endbox", _endBoxTitle, _endBoxText, [this] {
            start();
        }).render();

        if (_isGameRunning) {
            renderGame();
        }
    }

    void SimonSays::renderGame() {
        ui_elements::Window("Sequence Game").render(([this] {
            ui_elements::Centered(true, true, [this] {
                ImGui::PushFont(commons::Fonts::_header3);
                switch (_currentGameMode) {
                    case WATCH:
                        ImGui::Text("Sieh zu und versuch dir die Reihenfolge zu merken!");
                        break;
                    case REPEAT:
                        ImGui::Text("Wiederhole die Reihenfolge!");
                        break;
                }
                ImGui::PopFont();

                displayButtons();
            });
        }));
    }

    void SimonSays::start() {
        _currentGameMode = WATCH;
        _isGameRunning = true;
        _showEndBox = false;
        _sequenceButtonIterator = 0;
        _wasLastButtonOfSequence = false;

        _levelCounter = 0;
        _buttonsClickedSequence.clear();
        //Set all button states to false/0 (not lighting up)
        _buttonStates.fill(0);

        chooseNextRandomButton();
        nextLevel(); //first level
        showSequence();
    }

    void SimonSays::reset() {
        start();
    }

    void SimonSays::stop() {
        updateStatistics();
        _endBoxString = "Du hast eine Abfolge von " + std::to_string(_longestReproducedSequence) +
                        " Klicks richtig wiederholt!";
        _endBoxText = _endBoxString;
        _isGameRunning = false;
        _showEndBox = true;
        _endBoxTitle = "Falsch geklickt!";
    }

    void SimonSays::updateStatistics() {
        abstract_game::GameSessionManager::getCurrentSession()->addNewGameRunThrough("korrekte Antworten in Folge",
            _longestReproducedSequence);
    }

    void SimonSays::displayButtons() {
        ImGui::NewLine();

        for (int i{1}; i <= K_NUMBER_OF_BUTTONS; i++) {
            int const buttonID{i - 1};

            if ((i - 1) % 3) {
                ImGui::SameLine();
            } else {
                ImGui::NewLine();
            }
            switch (_currentGameMode) {
                case WATCH:

                    checkWaitTimeExpired();
                    checkLitUpExpired(
                        _buttonStates[buttonID]);
                //check, if Button is currently already lit up and then should be turned off
                    if (_wasLastButtonOfSequence) {
                        switchGameMode();
                    }

                    if (_buttonStates[buttonID]) {
                        //button is supposed to light up -> light button up by pushing accentColor
                        ImGui::PushStyleColor(ImGuiCol_Button, commons::ColorTheme::ACCENT_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, commons::ColorTheme::ACCENT_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, commons::ColorTheme::ACCENT_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_Text, commons::Colors::TRANSPARENT);
                    } else {
                        //button is not supposed to light up -> normal render color
                        ImGui::PushStyleColor(ImGuiCol_Button, commons::ColorTheme::PRIMARY_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, commons::ColorTheme::PRIMARY_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, commons::ColorTheme::PRIMARY_COLOR);
                        ImGui::PushStyleColor(ImGuiCol_Text, commons::Colors::TRANSPARENT);
                    }

                    if (ImGui::Button(std::to_string(buttonID).c_str(), ImVec2(200, 200))) {
                        //clicking is not supposed to have any effect in WATCH mode
                    }

                    ImGui::PopStyleColor(); //one time extra because of Hovered color

                    break;
                case REPEAT:

                    ImGui::PushStyleColor(ImGuiCol_Button, commons::ColorTheme::PRIMARY_COLOR);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, commons::ColorTheme::SECONDARY_COLOR);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));

                    if (ImGui::Button(std::to_string(buttonID).c_str(), ImVec2(200, 200))) {
                        isClickedInCorrectOrder(buttonID);
                    }

                    break;
            }

            ImGui::PopStyleColor(3);
        }
    }

    void SimonSays::isClickedInCorrectOrder(int const &buttonID) {
        if (buttonID ==
            _buttonsClickedSequence[_sequenceButtonIterator]) {
            //if clicked button is the right button to be clicked in sequence
            _correctClicksOfCurrentSequence++;
            playButtonSound(buttonID);
            _sequenceButtonIterator++; //make sure that the next button in line gets rated as correct now!
            if (_correctClicksOfCurrentSequence == _levelCounter) {
                //go to next level and switch back to watch gamemode
                _longestReproducedSequence = _correctClicksOfCurrentSequence;
                nextLevel();
                switchGameMode();
            }
        } else {
            commons::SoundPolice::safePlaySound(Sound::BEEP_FAIL);
            stop(); //wrong button clicked -> GAMEOVER!
        }
    }

    void SimonSays::chooseNextRandomButton() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distribution(0, K_NUMBER_OF_BUTTONS - 1);

        _buttonsClickedSequence.emplace_back(distribution(gen));
    }

    void SimonSays::showSequence() {
        int buttonForLoopIteration{0}; //to keep iterator of button in sequence in sync with the sequence displayed
        for (int button: _buttonsClickedSequence) {
            if ((_buttonStates[button] == 0) &&
                _canShowNextButtonInSequence && (_sequenceButtonIterator ==
                                                 buttonForLoopIteration) &&
                !_mustWait) {
                //if chosen button is not yet lit up AND no other button is currently lit up AND wait time between light ups is over, light it up!
                lightUp(_buttonStates[button], button); //light up button X by setting state of button X to 1/true
            }
            buttonForLoopIteration++;
        }
    }

    void SimonSays::lightUp(int &buttonState, int const &buttonID) {
        //Sets a button to be lit up for 800ms
        _stopHighlightingHere =
                std::chrono::steady_clock::now() + std::chrono::milliseconds(_lightUpDurationInMilliseconds);
        buttonState = 1;
        playButtonSound(buttonID);
        _canShowNextButtonInSequence = false;
        //another button is currently lit up, this variable assures no other button is being lit up at the same time
    }

    void SimonSays::checkLitUpExpired(int &buttonState) {
        //if Lighting up time is expired
        if ((buttonState == 1) && std::chrono::steady_clock::now() > _stopHighlightingHere) {
            buttonState = 0;
            waitInBetweenButtons();
        }
    }

    void SimonSays::nextLevel() {
        _levelCounter++;
    }

    void SimonSays::switchGameMode() {
        switch (_currentGameMode) {
            case WATCH:
                _currentGameMode = REPEAT;

                _correctClicksOfCurrentSequence = 0; //reset correctClicksCount for the new round
                _sequenceButtonIterator = 0;
                break;

            case REPEAT:
                _currentGameMode = WATCH;

                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                _wasLastButtonOfSequence = false; //reset sequence show endtime checker variable
                _sequenceButtonIterator = 0;
                chooseNextRandomButton(); //Sequence must be elongated by one button click

                showSequence();
                break;
        }
    }

    void SimonSays::moveOnToNextButton() {
        _sequenceButtonIterator++;
    }

    void SimonSays::waitInBetweenButtons() {
        _stopWaitingHere = std::chrono::steady_clock::now() + std::chrono::milliseconds(_waitDurationInMilliseconds);
        _mustWait = true;
    }

    void SimonSays::checkWaitTimeExpired() {
        if (_mustWait && (std::chrono::steady_clock::now() > _stopWaitingHere)) {
            _mustWait = false;

            if (_sequenceButtonIterator <= _levelCounter) {
                _canShowNextButtonInSequence = true;
                //Button ist wieder aus, also kann nun der nächste Button aufleuchten
                moveOnToNextButton();
                showSequence();
                if (_sequenceButtonIterator == _levelCounter) {
                    _wasLastButtonOfSequence = true;
                }
            }
        }
    }

    void SimonSays::playButtonSound(const int &buttonID) {
        switch (buttonID) {
            case 0:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 0.666f);
                break;
            case 1:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 0.777f);
                break;
            case 2:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 0.888f);
                break;
            case 3:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 0.999f);
                break;
            case 4:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.11f);
                break;
            case 5:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.222f);
                break;
            case 6:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.333f);
                break;
            case 7:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.444f);
                break;
            case 8:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.555f);
                break;
            default:
                commons::SoundPolice::safePlaySound(Sound::BEEP, 100, 1.0f);
                break;
        }
    }
} // games
