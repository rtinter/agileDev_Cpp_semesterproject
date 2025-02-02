#include "Window.hpp"

#include <imgui.h>

namespace ui_elements {
    // Constructor definition
    Window::Window(std::string const &name, bool *p_open, ImGuiWindowFlags const flags)
        : _name(name), _pOpen(p_open), _flags(flags) {
    }

    bool Window::begin() const {
        if (_name.empty()) {
            return false;
        }
        return ImGui::Begin(_name.c_str(), _pOpen, _flags);
    }

    void Window::end() {
        ImGui::End();
    }
} // ui_element
