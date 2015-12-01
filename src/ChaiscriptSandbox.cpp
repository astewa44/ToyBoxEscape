//
// Created by Anthony on 11/27/2015.
//

#include "ChaiscriptSandbox.hpp"

#include <memory>

using namespace std;

ChaiscriptSandbox::ChaiscriptSandbox(Engine *engine) : engine(engine) {
    go = make_shared<ChaiGameObject>(engine->chai, "data/scripts/go.chai");
}

void ChaiscriptSandbox::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->states.pop();
        return;
    }
    go->update();
}

void ChaiscriptSandbox::draw() {
    go->draw();
}
