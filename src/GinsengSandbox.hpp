//
// Created by Anthony on 8/31/2015.
//

#ifndef TOYBOXESCAPE_GINSENGSANDBOX_HPP
#define TOYBOXESCAPE_GINSENGSANDBOX_HPP

#include "Engine.hpp"
#include "Entcom.hpp"
#include "Components.hpp"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <json/json.h>

class GinsengSandbox {
private:
    Engine* engine = nullptr;

    DB db;

    sf::Clock clock;
    float accumulator;

    sf::View gridView;

    std::shared_ptr<chaiscript::ChaiScript> chai;

    void loadLevel(std::string fname);
    void load(Json::Value json);

public:
    GinsengSandbox(Engine* engine);
    void update();
    void draw();
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return true; }
};


#endif //TOYBOXESCAPE_GINSENGSANDBOX_HPP
