//
// Created by Anthony on 9/5/2015.
//

#include "Ai.hpp"

using namespace std;

void Components::update_ais(Engine* engine, DB &db)
{
    for (auto ent : db.query<AIComponent>())
    {
        get<1>(ent).data().update(engine, db, get<0>(ent));
    }
}

Components::PlayerAI::PlayerAI(EntID me)
{
    b2Body* body = me.get<DynamicBody>().data().Body;

    for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
    {
        fixture->SetFriction(0.f);
        fixture->SetRestitution(0.f);
    }
    body->SetFixedRotation(true);

}

void Components::PlayerAI::operator()(Engine* engine, DB &db, EntID me, Components::AIComponent &my_ai)
{
    b2Body* body = me.get<DynamicBody>().data().Body;

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
            body->ApplyLinearImpulse(b2Vec2(0, 20), body->GetWorldCenter(), true);
            rising = sf::seconds(0.10);
        }
    }

    sf::Time t = clock.restart();

    if (rising.asSeconds() > 0 && (engine->isKeyDown(sf::Keyboard::Up) || engine->isKeyDown(sf::Keyboard::W))) {
        rising -= t;
        body->SetGravityScale(0);
    } else {
        rising = sf::seconds(0);
        body->SetGravityScale(1);
    }

    b2Vec2 vel = body->GetLinearVelocity();
    float dvel = 0;
    float velocity = 8.f;

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
}

