#include "Dashboard.hpp"

#include <imgui.h>

#include "Centered.hpp"
#include "Fonts.hpp"
#include "Window.hpp"


namespace views {
    void renderCategory(String const &categoryName, std::vector<UniqueTile> &tiles) {
        ImGui::Spacing();

        ImGui::PushFont(commons::Fonts::_header2);
        ImGui::TextUnformatted(categoryName.c_str());
        ImGui::PopFont();

        ImGui::Spacing();

        int count{0};
        for (UniqueTile &tile: tiles) {
            count++;
            if (tile)
                tile->render();
            if (count == 4) {
                ImGui::NewLine();
                count = 0;
            } else {
                ImGui::SameLine();
            }
        }
    }

    void Dashboard::addTileToCategory(std::string const &category,
                                      UniqueTile &tile) {
        _categoryTiles[category].emplace_back(std::move(tile));
    }

    //add tiles to category
    void Dashboard::addTilesToCategory(std::string const &category, std::vector<UniqueTile> &tiles) {
        for (auto &tile: tiles) {
            addTileToCategory(category, tile);
        }
    }

    void Dashboard::render() {
        ui_elements::Window("Dashboard").render([this] {
            for (auto &[fst, snd]: _categoryTiles) {
                renderCategory(fst, snd);
            }
        });
    }
} // views
