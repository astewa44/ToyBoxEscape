//
// Created by Anthony on 8/31/2015.
//

#ifndef TOYBOXESCAPE_GINSENGSANDBOX_HPP
#define TOYBOXESCAPE_GINSENGSANDBOX_HPP

#include "Engine.hpp"
#include "Entcom.hpp"
#include "Components.hpp"

class GinsengSandbox {
private:
    Engine* engine = nullptr;

    DB db;

    sf::Clock clock;
    float accumulator;

    sf::View gridView;

public:
    GinsengSandbox(Engine* engine);
    void update();
    void draw();
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return true; }
    double function(int i, double j);
};


#endif //TOYBOXESCAPE_GINSENGSANDBOX_HPP
