//
// Created by Anthony on 7/24/2015.
//

#ifndef TOYBOXESCAPE_PHYSICSSANDBOX_HPP
#define TOYBOXESCAPE_PHYSICSSANDBOX_HPP

#include "Entcom.hpp"
#include "Engine.hpp"
#include "DebugDraw.hpp"
#include "Components.hpp"
#include "Sensors.hpp"

#include <sfml/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <memory>

class PhysicsSandbox
{
private:
    Engine* engine = nullptr;

    sf::View gridView;
    sf::RectangleShape majorX, minorX;
    sf::RectangleShape majorY, minorY;

    std::shared_ptr<b2World> world;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;

    b2Body* groundBody;
    b2Body* boxBody;
    b2Body* platformBody;
    b2Body* body;

    sf::RectangleShape groundRect;
    sf::RectangleShape boxRect;
    sf::RectangleShape platformRect;
    sf::RectangleShape bodyRect;

    float b2DrawFactor;

    sf::Clock clock;
    float accumulator;

    sf::Time rising = sf::Time::Zero;

    DebugDraw* debugDraw;

public:
    PhysicsSandbox(Engine* engine);
    void update();
    void draw();
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return true; }
};

#endif //TOYBOXESCAPE_PHYSICSSANDBOX_HPP
