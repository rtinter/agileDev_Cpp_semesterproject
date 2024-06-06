#pragma once

#include "Header.hpp"
#include "Reaction.hpp"
#include "Scene.hpp"

#include <string>

template<typename T>
class GameScene : public Scene {
    views::Header header;
    std::unique_ptr<T> _game;
public:
    GameScene<T>();
    virtual void render() override;
    virtual std::string getName() const override;
};

template class GameScene<games::Reaction>;