//
// Created by Anthony on 7/24/2015.
//

#include "PhysicsSandbox.hpp"
#include "Util.hpp"

#include <iostream>

using namespace std;

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        GroundSensor* fixtureUserData;
        void* userData;

        //check if fixture A was the foot sensor
        userData = contact->GetFixtureA()->GetUserData();
        if (userData != NULL) {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts++;
        }

        //check if fixture B was the foot sensor
        userData = contact->GetFixtureB()->GetUserData();
        if (userData != NULL) {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts++;
        }
    }

    void EndContact(b2Contact* contact) {
        GroundSensor* fixtureUserData;
        void* userData;

        //check if fixture A was the foot sensor
        userData = contact->GetFixtureA()->GetUserData();
        if (userData != NULL) {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts--;
        }
        //check if fixture B was the foot sensor
        userData = contact->GetFixtureB()->GetUserData();
        if (userData != NULL) {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts--;
        }
    }
};

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

    /*
    EntID ent = db.makeEntity();
    PhysicsWorld physicsWorld{make_shared<b2World>(b2Vec2(0.f, -9.81f * 8)), 1.f / 60.f, 8, 3};
    physicsWorld.World->SetContactListener(new ContactListener());
    DebugDraw* debugDraw = new DebugDraw(engine->window);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    physicsWorld.World->SetDebugDraw(debugDraw);
    db.makeComponent(ent, physicsWorld);
    */

    world = std::make_shared<b2World>(b2Vec2(0.f, -9.81f * 8));
    world->SetContactListener(new ContactListener());
    debugDraw = new DebugDraw(engine->window, 32.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    world->SetDebugDraw(debugDraw);
    timeStep = 1.f / 60.f;
    velocityIterations = 8;
    positionIterations = 3;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.f, -10.f);
    groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(25.f, 10.f);
    groundBody->CreateFixture(&groundBox, 0.f);

    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(2.f, 1.f);
    boxBody = world->CreateBody(&boxBodyDef);

    b2PolygonShape boxBox;
    boxBox.SetAsBox(1.f, 1.f);
    boxBody->CreateFixture(&boxBox, 0.f);

    b2BodyDef platformBodyDef;
    platformBodyDef.position.Set(-7.f, 4.f);
    platformBody = world->CreateBody(&platformBodyDef);

    b2PolygonShape platformBox;
    platformBox.SetAsBox(5.f, 0.1f);
    platformBody->CreateFixture(&platformBox, 0.f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.f, 2.f);
    bodyDef.fixedRotation = true;
    body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.f, 1.8f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.f;
    body->CreateFixture(&fixtureDef);

    b2PolygonShape sensorShape;
    sensorShape.SetAsBox(0.9f, 0.2f, b2Vec2(0.f, -1.8f), 0);

    b2FixtureDef sensorFixtureDef;
    sensorFixtureDef.shape = &sensorShape;
    sensorFixtureDef.isSensor = true;
    sensorFixtureDef.userData = new GroundSensor();

    b2Fixture* sensorFixture = body->CreateFixture(&sensorFixtureDef);
    
    b2DrawFactor = 30;

    groundRect.setFillColor(sf::Color(150, 75, 0));
    b2AABB aabb = groundBody->GetFixtureList()->GetAABB(0);
    groundRect.setSize(sf::Vector2f(aabb.GetExtents().x * 2 * b2DrawFactor, aabb.GetExtents().y * 2 * b2DrawFactor));
    groundRect.setOrigin(sf::Vector2f(aabb.GetExtents().x * b2DrawFactor, aabb.GetExtents().y * b2DrawFactor));

    boxRect.setFillColor(sf::Color(150, 75, 0));
    aabb = boxBody->GetFixtureList()->GetAABB(0);
    boxRect.setSize(sf::Vector2f(aabb.GetExtents().x * 2 * b2DrawFactor, aabb.GetExtents().y * 2 * b2DrawFactor));
    boxRect.setOrigin(sf::Vector2f(aabb.GetExtents().x * b2DrawFactor, aabb.GetExtents().y * b2DrawFactor));

    bodyRect.setFillColor(sf::Color(126, 192, 238));
    aabb = body->GetFixtureList()->GetNext()->GetAABB(0);
    bodyRect.setSize(sf::Vector2f(aabb.GetExtents().x * 2 * b2DrawFactor, aabb.GetExtents().y * 2 * b2DrawFactor));
    bodyRect.setOrigin(sf::Vector2f(aabb.GetExtents().x * b2DrawFactor, aabb.GetExtents().y * b2DrawFactor));

    accumulator = 0.f;
}

void PhysicsSandbox::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape)) {
        engine->states.pop();
        return;
    }

    bool onGround = false;
    for (b2Fixture* f = body->GetFixtureList(); f != NULL; f = f->GetNext()) {
        void * userData = f->GetUserData();
        if (userData != NULL) {
            GroundSensor* data = (GroundSensor*)userData;
            if (!strcmp(data->type, "GroundSensor"))
                onGround = (data->numFootContacts > 0);
        }
    }

    if (engine->wasKeyPressed(sf::Keyboard::Up) || engine->wasKeyPressed(sf::Keyboard::W)) {
        if (onGround) {
            body->ApplyLinearImpulse(b2Vec2(0, 100), body->GetWorldCenter(), true);
            rising = sf::seconds(0.10);
        }
    }

    sf::Time t = clock.restart();

    accumulator += t.asSeconds();

    if (rising.asSeconds()>0 && (engine->isKeyDown(sf::Keyboard::Up) || engine->isKeyDown(sf::Keyboard::W))) {
        rising -= t;
        body->SetGravityScale(0);
    } else {
        rising = sf::seconds(0);
        body->SetGravityScale(1);
    }

    b2Vec2 vel = body->GetLinearVelocity();
    float dvel = 0;
    float velocity = 15.f;

    if (engine->isKeyDown(sf::Keyboard::Left) || engine->isKeyDown(sf::Keyboard::A))
        dvel -= 1;

    if (engine->isKeyDown(sf::Keyboard::Right) || engine->isKeyDown(sf::Keyboard::D))
        dvel += 1;

    if (dvel < 0)
        dvel = b2Max(vel.x - 0.1f, -velocity);
    else if (dvel > 0)
        dvel = b2Min(vel.x + 0.1f, velocity);
    else if (onGround) {
        dvel = 0.99f * vel.x;
    }

    float impulse;

    if (dvel != 0) {
        float dV = dvel - vel.x;
        impulse = body->GetMass() * dV;
        body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
    }

    while (accumulator >= timeStep)
    {
        world->Step(timeStep, velocityIterations, positionIterations);

        b2Vec2 position = groundBody->GetPosition();
        groundRect.setPosition(sf::Vector2f(position.x * b2DrawFactor, -position.y * b2DrawFactor));
        position = boxBody->GetPosition();
        boxRect.setPosition(sf::Vector2f(position.x * b2DrawFactor, -position.y * b2DrawFactor));
        position = body->GetPosition();
        bodyRect.setPosition(sf::Vector2f(position.x * b2DrawFactor, -position.y * b2DrawFactor));

        accumulator -= timeStep;
    }
}

void PhysicsSandbox::draw() {
    engine->window.setView(gridView);
    world->DrawDebugData();
    /*
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
    engine->window.draw(boxRect);
    */
    engine->window.setView(engine->window.getDefaultView());
}
