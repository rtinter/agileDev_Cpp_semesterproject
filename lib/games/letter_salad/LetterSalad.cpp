//
// Created by Benjamin Puhani on 08.06.2024.
//

#include <iostream>
#include "LetterSalad.hpp"
#include "Window.hpp"
#include "Centered.hpp"
#include <algorithm>
#include "Fonts.hpp"
#include "TextCentered.hpp"
#include "imgui_internal.h"

#define EMPTY_CELL "_"

namespace game {

std::string LetterSalad::getName() const {
    return "Letter Salad";
}

CharVector2D LetterSalad::_gameField = {20, {20, {EMPTY_CELL, false}}};
std::vector<Coordinates> LetterSalad::_currentLine = {};
std::vector<std::pair<std::string, bool>> LetterSalad::_wordList = {
    {"Hallo", true},
    {"Welt", false}
};

void LetterSalad::render() {

    if (_gameField[0][0].first == EMPTY_CELL) {
        LetterSalad::randomizeGameField();
    }

    ui_elements::Window("LetterSalad Game").render([this]() {

      ImGui::BeginListBox("##textList",
                          ImVec2(300, ImGui::GetWindowHeight()-100));

      for (auto &wordPair : _wordList) {
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
          ImGui::Checkbox(wordPair.first.c_str(),
                          &wordPair.second);
          ImGui::PopItemFlag();
      }

      ImGui::EndListBox(); // ##textList
      ImGui::SameLine();
      ImGui::Spacing();
      ImGui::SameLine();
      this->renderGameField();
    });
}

void LetterSalad::onHover(Coordinates const &coords) {

    static Coordinates lastHoveredCell{-1, -1};

    auto newlines{getLine(_firstSelectedCell, coords)};

    if (newlines.empty()) {
        return;
    }

    // check if the hovered cell is not the last hovered cell
    if (lastHoveredCell.x != -1 && lastHoveredCell != coords) {
        std::vector<Coordinates> difference;
        // remove all elements that are now not hovered anymore
        std::set_difference(_currentLine.begin(), _currentLine.end(),
                            newlines.begin(), newlines.end(),
                            std::back_inserter(difference));
        for (auto &lineE : difference) {
            deSelectBox(lineE);
        }

    } else {
        _selectedWord = "";
        _currentLine = getLine(_firstSelectedCell, coords);

        for (auto &lineE : _currentLine) {
            selectBox(lineE);
            _selectedWord += _gameField[lineE.y][lineE.x].first;
        }
    }

    lastHoveredCell = coords;
}

void LetterSalad::clickCell(Coordinates const &coords) {

    // if the first has not been selected yet
    if (!_isFirstCellSelected) {
        // then this is the first cell selected
        _isFirstCellSelected = true;
        _firstSelectedCell = coords;
        _selectedWord = _gameField[coords.y][coords.x].first;
        selectBox(coords);
    } else if (!_isSecondCellSelected) {
        // if the first cell has been selected
        // then this is the second cell selected
        _isSecondCellSelected = true;
        _secondSelectedCell = coords;
        pairSelected();
        for (auto &lineE : getLine(_firstSelectedCell, _secondSelectedCell)) {
            selectBox(lineE);
        }
        // check if the selected elements are correct, if not reset
        resetSelectedPair();
    }

}

void LetterSalad::renderGameField() {
    ImGui::BeginChild("##gameField",
                      ImVec2(900, 900));
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign,
                        ImVec2(0.5f, 0.5f));

    for (int y = 0; y < _gameField.size(); y++) {
        auto &row = _gameField[y];

        for (int x = 0; x < row.size(); x++) {
            // Always print on the same line but the first cell
            if (x > 0) ImGui::SameLine();

            // PushID is used to ensure that each cell has a unique ID
            ImGui::PushID(y * 20+x);
            if (ImGui::Selectable(_gameField[y][x].first.c_str(),
                                  _gameField[y][x].second,
                                  ImGuiSelectableFlags_AllowOverlap,
                                  ImVec2(20, 20))) {
                // Toggle clicked cell if clicked
                clickCell({y, x});
            }

            // run if the first cell has been selected
            // and another cell is hovered
            if (_isFirstCellSelected && ImGui::IsItemHovered()) {
                onHover({y, x});
            }

            ImGui::BeginDisabled(true);
            ImGui::EndDisabled();

            ImGui::PopID();
        }
    }
    ImGui::PopStyleVar();
    ImGui::BeginChild("##selectedWord",
                      ImVec2(500, 40));
    ImGui::PushFont(commons::Fonts::_header2);
    ImGui::TextDisabled(_selectedWord.c_str());
    ImGui::PopFont();
    ImGui::EndChild(); // ##selectedWord

    ImGui::EndChild(); // ##gameField
}

void LetterSalad::resetSelectedPair() {
    _selectedWord = "";
    _isFirstCellSelected = false;
    _isSecondCellSelected = false;
    for (auto &lineE : getLine(_firstSelectedCell, _secondSelectedCell)) {
//        deSelectBox(lineE);
    }
    _firstSelectedCell = {-1, -1};
    _secondSelectedCell = {-1, -1};
    _currentLine.clear();
}

/**
 * Get the coordinates of the cells between the start and end cell.
 * Diagonally, horizontally or vertically.
 * Basing on the Bresenham's line algorithm.
 */
std::vector<Coordinates> LetterSalad::getLine(Coordinates const &start,
                                              Coordinates const &end) {
    std::vector<Coordinates> linePoints;
    int x1{start.x};
    int y1{start.y};
    int x2{end.x};
    int y2{end.y};

    int dx{std::abs(x2-x1)};
    int sx{x1 < x2 ? 1 : -1};
    int dy{-std::abs(y2-y1)};
    int sy{y1 < y2 ? 1 : -1};
    int err{dx+dy};
    int e2; // Fehlerwert e_xy

    // make sure to only draw elements
    // if the start and end cell are in one line
    // diagonally or horizontally or vertically
    bool isDiagonal = {std::abs(dx) == std::abs(dy)};
    bool isHorizontal = {dy == 0};
    bool isVertical = {dx == 0};

    // return if the cells are not in one line
    if (!isDiagonal && !isHorizontal && !isVertical) {
        return linePoints;
    }

    while (true) {
        linePoints.emplace_back(y1, x1);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { // Fehler akkumulierte sich in horizontaler Richtung
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { // Fehler akkumulierte sich in vertikaler Richtung
            err += dx;
            y1 += sy;
        }
    }
    return linePoints;
}

void LetterSalad::selectBox(Coordinates const &coords) {
    if (coords.x < 0 || coords.y < 0 || coords.x >= 20 || coords.y >= 20) {
        std::cerr << "Invalid coordinates" << std::endl;
        std::cerr << coords.y << " " << coords.x << std::endl;
        return;
    }
    _gameField[coords.y][coords.x].second = true;
}

void LetterSalad::deSelectBox(Coordinates const &coords) {
    if (coords.x < 0 || coords.y < 0 || coords.x >= 20 || coords.y >= 20) {
        std::cerr << "Invalid coordinates" << std::endl;
        std::cerr << coords.y << " " << coords.x << std::endl;
        return;
    }
    _gameField[coords.y][coords.x].second = false;
}

void LetterSalad::pairSelected() {
}

void LetterSalad::renderGame() {

}

void LetterSalad::update() {

}

void LetterSalad::stop() {
    Game::stop();
}
void LetterSalad::start() {

}
void LetterSalad::reset() {

}
void LetterSalad::updateStatistics() {

}
void LetterSalad::randomizeGameField() {
    for (auto &row : _gameField) {
        for (auto &x : row) {
            // TODO @bpuhani check if an field already has a letter
            x.first = static_cast<char>(rand() % 23+65);
        }
    }
}

} // namespace game