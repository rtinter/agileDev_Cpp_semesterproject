#include "AimTrainer.hpp"
#include "Window.hpp"

int randomPos(int n){
    return rand() % n + 1;
}

void games::AimTrainer::spawnBlobs(){
    const auto size = ImGui::GetWindowSize();
    int windowWidth {static_cast<int>(size.x)};
    int windowHeight {static_cast<int>(size.y)};

    for(int i {0}; i < _spawnAmount; ++i){
        int x { randomPos(windowWidth) };
        if(x < 240)
            x = 240;
        if(x > windowWidth)
            x = windowWidth - 240;
        int y { randomPos(windowHeight) };

        if(y < 240)
            y = 240;
        if(y > windowHeight)
            y = windowHeight - 240;

        _currentBlobs.emplace_back(
                x,
                y,
                _currentBlobs.size() + 1
        );
    }
}

void games::AimTrainer::updateBlobs(){
    // remove blobs that are to be disposed
    int missed {0};
    _currentBlobs.erase(
            std::remove_if(
                    _currentBlobs.begin(),
                    _currentBlobs.end(),
                    [&missed](aim_trainer::Blob &b){

                        bool dispose {b.canBeDisposed()};
                        if(dispose)
                            missed++;
                        return dispose;
                    }), _currentBlobs.end());

    _missedCounter += missed;

    // decrease blob size
    for(auto &blob : _currentBlobs){
        blob.decrease((1.0/(200)));
    }
}

void games::AimTrainer::render() {
    ui_elements::InfoBox(_gameID, _showInfobox, _gameName, _gameDescription, _gameRules, _gameControls, [this] {
        start();
    }).render();

    if(ImGui::IsMouseClicked(0)){
        auto const mousePos {ImGui::GetMousePos()};

        std::vector<aim_trainer::Blob> blobsToDelete;
        bool hit {false};
        _currentBlobs.erase(
                std::remove_if(
                        _currentBlobs.begin(),
                        _currentBlobs.end(),
                        [this, &hit, mousePos](aim_trainer::Blob &b){

                            const auto coords {b.getCoords()};
                            const float dx {coords.x - mousePos.x};
                            const float dy {coords.y - mousePos.y};
                            const float dist {dx * dx + dy * dy};

                            // increment successful clicks of blobs
                            bool inCircle {dist <= b.getRadius() * b.getRadius()};
                            if(inCircle){
                                _successCounter++;
                                hit = true;
                            }
                            return inCircle;
                        }), _currentBlobs.end());

        if(!hit && _timer){
            _timer->reduceTime(hit ? 2 : 2 * (_missedFactor));
            _missedFactor += 0.25;
            _missedCounter++;
        }
    }

    ui_elements::Overlay("Endbox", _showEndbox).render([this]() {
        ImGui::PushFont(commons::Fonts::_header2);
        ui_elements::TextCentered(std::move(_endboxTitle));
        ImGui::PopFont();
        ui_elements::TextCentered(std::move(_endboxText));

        ui_elements::Centered(true, true, [this]() {
            if (ImGui::Button("Versuch es nochmal")) {
                reset();
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

void games::AimTrainer::renderGame() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, sf::Color(200, 200, 200, 255));
    ui_elements::Window("Aim Trainer").render([this] {
        _timer->render();
        if(_timer->isExpiredNow()){
            _isGameRunning = false;
            _showEndbox = true;
        }
        _elapsedTime = _clock.getElapsedTime();
        if (_elapsedTime.asSeconds() >= 2) {
            // Restart the clock to measure the next interval
            spawnBlobs();
            _clock.restart();
            _spawnAmount += 0.15;
        }

        ImGui::PushFont(commons::Fonts::_header2);
        ui_elements::TextCentered(std::move(("Hit: " + std::to_string(_successCounter)).c_str()));
        ImGui::PopFont();
        ui_elements::TextCentered(std::move(("Missed: " + std::to_string(_missedCounter)).c_str()));
        updateBlobs();
        for(const auto &blob : _currentBlobs){
            blob.render();
        }
    });
    ImGui::PopStyleColor();
}

void games::AimTrainer::reset() {
    _successCounter = 0;
    _missedCounter = 0;
    _missedFactor = 1;
    _spawnAmount = 1;
    _currentBlobs.clear();
}

void games::AimTrainer::start() {
    _isGameRunning = true;
    _showEndbox = false;

    _timer = std::make_unique<ui_elements::Timer>("Aim Trainer", 60);
    _timer->start();

    spawnBlobs();
}

void games::AimTrainer::stop() {
}

void games::AimTrainer::updateStatistics() {
}

std::string games::AimTrainer::getName() const {
    return "AimTrainer";
}

games::AimTrainer::AimTrainer() : Game(abstract_game::GameID::AIM_TRAINER) {
    _gameName = "Aim Trainer";
    _gameDescription = R"(
Willkommen in der Welt von "Circle Strike", dem fesselnden Aim Trainer,
der deine Zielpräzision auf die Probe stellt und auf die nächste Stufe hebt.
Perfekt für Gamer, die ihre Fähigkeiten schärfen wollen, oder für jeden,
der seine Mausgenauigkeit und Reaktionszeit verbessern möchte.
"Circle Strike" bietet eine einfache, doch herausfordernde Spielmechanik,
die dich nicht mehr loslassen wird.
    )";
    _gameRules = "Klicke nur auf die Kreise, nirgendswo anders hin!";
    _gameControls = "";
}
