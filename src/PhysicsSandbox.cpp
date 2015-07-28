//
// Created by Anthony on 7/24/2015.
//

#include "PhysicsSandbox.hpp"

#include <iostream>

using namespace std;

PhysicsSandbox::PhysicsSandbox(Engine *engine) : engine(engine) {
    sf::Vector2u wSize = engine->window.getSize();
    gridView.reset(sf::FloatRect(wSize.x / -2.f, wSize.y / -2.f, wSize.x, wSize.y));

    majorX.setSize(sf::Vector2f(wSize.x, 8));
    majorX.setFillColor(sf::Color::Black);
    majorX.setPosition(wSize.x / -2.f, -4.f);
    minorX.setSize(sf::Vector2f(2, 50));
    minorX.setFillColor(sf::Color(128, 128, 128));

    majorY.setSize(sf::Vector2f(8, wSize.y));
    majorY.setFillColor(sf::Color::Black);
    majorY.setPosition(-4.f, wSize.y / -2.f);
    minorY.setSize(sf::Vector2f(50, 2));
    minorY.setFillColor(sf::Color(128, 128, 128));

    world = std::make_shared<b2World>(b2Vec2(0.f, -9.81f));
    timeStep = 1.f / 60.f;
    velocityIterations = 8;
    positionIterations = 3;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.f, -10.f);

    groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(25.f, 10.f);

    groundBody->CreateFixture(&groundBox, 0.f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.f, 20.f);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.f, 1.8f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.1f;

    body->CreateFixture(&fixtureDef);
    
    b2DrawFactor = 30;

    groundRect.setFillColor(sf::Color(150, 75, 0));
    b2AABB aabb = groundBody->GetFixtureList()[0].GetAABB(0);
    groundRect.setSize(sf::Vector2f(aabb.GetExtents().x * 2 * b2DrawFactor, aabb.GetExtents().y * 2 * b2DrawFactor));
    groundRect.setOrigin(sf::Vector2f(aabb.GetExtents().x * b2DrawFactor, aabb.GetExtents().y * b2DrawFactor));

    bodyRect.setFillColor(sf::Color(126, 192, 238));
    aabb = body->GetFixtureList()[0].GetAABB(0);
    bodyRect.setSize(sf::Vector2f(aabb.GetExtents().x * 2 * b2DrawFactor, aabb.GetExtents().y * 2 * b2DrawFactor));
    bodyRect.setOrigin(sf::Vector2f(aabb.GetExtents().x * b2DrawFactor, aabb.GetExtents().y * b2DrawFactor));
}

void PhysicsSandbox::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape)) {
        engine->states.pop();
        return;
    }

    if (engine->wasKeyPressed(sf::Keyboard::Up) || engine->wasKeyPressed(sf::Keyboard::W))
        body->ApplyLinearImpulse(b2Vec2(0, 50), body->GetWorldCenter(), true);

    sf::Time t = clock.getElapsedTime();

    if (t.asSeconds() >= timeStep)
    {
        world->Step(timeStep, velocityIterations, positionIterations);

        b2Vec2 position = groundBody->GetPosition();
        groundRect.setPosition(sf::Vector2f(position.x * b2DrawFactor, -position.y * b2DrawFactor));
        position = body->GetPosition();
        bodyRect.setPosition(sf::Vector2f(position.x * b2DrawFactor, -position.y * b2DrawFactor));

        clock.restart();
    }
}

void PhysicsSandbox::draw() {
    engine->window.setView(gridView);
    engine->window.clear(sf::Color::White);
    sf::Vector2u wSize = engine->window.getSize();
    wSize.x /= 100; wSize.y /= 100;

    engine->window.draw(majorX);
    engine->window.draw(majorY);

    for (int i = int(wSize.x) / -2; i <= int(wSize.x) / 2; i++)
    {
        if (i == 0)
            continue;

        minorX.setPosition(i * 100 - 1, -25.f);
        engine->window.draw(minorX);
    }

    for (int i = int(wSize.y) / -2; i <= int(wSize.y) / 2; i++)
    {
        if (i == 0)
            continue;

        minorY.setPosition(-25.f, i * 100 - 1);
        engine->window.draw(minorY);
    }

    engine->window.draw(groundRect);
    engine->window.draw(bodyRect);

    engine->window.setView(engine->window.getDefaultView());
}
