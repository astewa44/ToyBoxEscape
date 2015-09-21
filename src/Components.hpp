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
            bodyDef.position.Set(x / 2.f, y / 2.f);
            Body = physicsWorld.World->CreateBody(&bodyDef);

            b2PolygonShape box;
            box.SetAsBox(w / 2.f, h / 2.f);
            Body->CreateFixture(&box, 0.f);
        }
    };

    struct FootSensor
    {
        b2Fixture* sensorFixture;

        FootSensor(b2Body* body)
        {
            b2PolygonShape sensorShape;
            sensorShape.SetAsBox(body->GetFixtureList()->GetAABB(0).GetExtents().x * 0.9f, 0.2f, b2Vec2(0.f, -body->GetFixtureList()->GetAABB(0).GetExtents().y), 0);

            b2FixtureDef sensorFixtureDef;
            sensorFixtureDef.shape = &sensorShape;
            sensorFixtureDef.isSensor = true;
            sensorFixtureDef.userData = new GroundSensor();

            sensorFixture = body->CreateFixture(&sensorFixtureDef);
        }

        bool OnGround()
        {
            GroundSensor* data = (GroundSensor*)sensorFixture->GetUserData();
            return (data->numFootContacts > 0);
        }
    };
}

#endif //TOYBOXESCAPE_COMPONENTS_HPP
