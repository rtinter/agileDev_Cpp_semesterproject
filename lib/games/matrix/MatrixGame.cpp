#include "MatrixGame.hpp"
#include "Window.hpp"
#include "InfoBox.hpp"
#include "Overlay.hpp"
#include "Fonts.hpp"
#include "TextCentered.hpp"
#include "Centered.hpp"
#include "SceneManager.hpp"
#include "DashboardScene.hpp"

namespace games {

    MatrixGame::MatrixGame() : Game(abstract_game::GameID::MATRIX) {
        _gameName = "Matrix";
        _gameDescription =
                "Das Spiel 'Matrix' trainiert das räumliche Vorstellungsvermögen";
        _gameRules = "Finde in der unteren Zeile die gedrehte/gespiegelte Version der oberen Matrix";
        _gameControls = "Linke Maustaste: Klicke auf die richtige Matrix in der unteren Zeile";
    }

    void MatrixGame::render() {
        ui_elements::InfoBox(_gameID, _showInfobox, _gameName, _gameDescription, _gameRules, _gameControls, [this] {
            start();
        }).render();

        ui_elements::Overlay("Endbox", _showEndbox).render([this]() {
            ImGui::PushFont(commons::Fonts::_header2);
            ui_elements::TextCentered(std::move(_endboxTitle));
            ImGui::PopFont();
            ui_elements::TextCentered(std::move(_endboxText));

            ui_elements::Centered(true, true, [this]() {
                if (ImGui::Button("Versuch es nochmal")) {
                    start();
                }

                if (ImGui::Button("Zurück zur Startseite")) {
                    scene::SceneManager::getInstance().switchTo(std::make_unique<scene::DashboardScene>());
                }
            });
        });

        if (_isGameRunning) {
            renderGame();
        }
    }

    void MatrixGame::renderGame() {

        ui_elements::Window("Matrix Game").render([this] {
            ImGuiStyle &style{ImGui::GetStyle()};
            ImVec4 oldImGuiCol_WindowBg = style.Colors[ImGuiCol_WindowBg];
            style.Colors[ImGuiCol_WindowBg] = commons::Colors::BLACK;
            _timer.render();
            _mainMatrix.render(50.f);
            ImGui::NewLine();
            ImGui::NewLine();
            for (Matrix matrix: _allMirroredVersions) {
                matrix.render(30.f);
                ImGui::NewLine();
                ImGui::NewLine();
            }

            for (Matrix matrix: _allRotatedVersions) {
                matrix.render(10.f);
                ImGui::NewLine();
                ImGui::NewLine();
            }

            if (_timer.isExpiredNow()) {
                stop();
            }

        });
    }

    void MatrixGame::stop() {
        _endboxString =
                "Richtige: " + std::to_string(_numberOfCorrectClicksInTotal) + "\nLängster Streak: " +
                std::to_string(_longestStreak);
        _endboxText = _endboxString.c_str();
        _isGameRunning = false;
        _showEndbox = true;
        _endboxTitle = "Zeit abgelaufen!";
    }

    void MatrixGame::start() {
        reset();
        _mainMatrix.init();
        _allMirroredVersions = _mainMatrix.getAllMirroredVersions();
        _allRotatedVersions = _mainMatrix.getAllRotatedVersions();
        _isGameRunning = true;
        _showEndbox = false;
        _timer.start();
    }

    void MatrixGame::reset() {

    }

    void MatrixGame::updateStatistics() {

    }
} // games