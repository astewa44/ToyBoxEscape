//
// Created by Anthony on 8/30/2015.
//

#ifndef TOYBOXESCAPE_COMPONENTS_HPP
#define TOYBOXESCAPE_COMPONENTS_HPP

#include <memory>

#include <Box2D/Box2D.h>
#include <sfml/Graphics.hpp>

#include "Sensors.hpp"

namespace Components
{
    struct PhysicsWorld
    {
        std::shared_ptr<b2World> World;
        float32 TimeStep;
        int32 VelocityIterations;
        int32 PositionIterations;
        sf::Clock Clock;
        float accumulator = 0;

        PhysicsWorld(b2Vec2 gravity, float32 timeStep, int32 velIter, int32 posIter)
        {
            World = std::make_shared<b2World>(gravity);
            TimeStep = timeStep;
            VelocityIterations = velIter;
            PositionIterations = posIter;
        }

        void Update()
        {
            sf::Time t = Clock.restart();
            accumulator += t.asSeconds();

            while (accumulator >= TimeStep)
            {
                World->Step(TimeStep, VelocityIterations, PositionIterations);
                accumulator -= TimeStep;
            }
        }
    };

    struct DynamicBody
    {
        b2Body* Body;

        DynamicBody(PhysicsWorld &physicsWorld, float32 x, float32 y, float32 w, float32 h)
        {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(x, y);
            Body = physicsWorld.World->CreateBody(&bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(w / 2.f, h / 2.f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.f;
            Body->CreateFixture(&fixtureDef);
        }
    };

    struct StaticBody
    {
        b2Body* Body;

        StaticBody(PhysicsWorld &physicsWorld, float32 x, float32 y, float32 w, float32 h)
        {
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position.Set(x, y);
            Body = physicsWorld.World->CreateBody(&bodyDef);

            b2PolygonShape box;
            box.SetAsBox(w / 2.f, h / 2.f);
            Body->CreateFixture(&box, 0.f);
        }
    };
}

#endif //TOYBOXESCAPE_COMPONENTS_HPP
