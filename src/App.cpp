#include "App.hpp"
#include "StyleManager.hpp"
#include "SceneManager.hpp"
#include "../lib/games/color_match/ColorMatch.hpp"

#include <imgui-SFML.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


const int App::WINDOW_WIDTH{1920};
const int App::WINDOW_HEIGHT{1080};
const std::string App::TILE{"Human Benchmark"};
const int App::FRAME_RATE{60};


void App::start() {
    sf::RenderWindow window(sf::VideoMode(App::WINDOW_WIDTH, WINDOW_HEIGHT), App::TILE);
    window.setFramerateLimit(App::FRAME_RATE);

    //init singleton and start Dashboard
    scene::SceneManager &sceneManager{scene::SceneManager::getInstance()};
    sceneManager.addDefaultScenes();

    if (!ImGui::SFML::Init(window)) {
        // Initialisierung fehlgeschlagen
        return;
    }

    //load the styleManager to adjust Colors etc.
    commons::StyleManager::loadStyle();
    sf::Clock deltaClock;

    while (window.isOpen()) {
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sceneManager.render();

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
