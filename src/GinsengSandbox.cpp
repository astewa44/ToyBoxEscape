//
// Created by Anthony on 8/31/2015.
//

#include "GinsengSandbox.hpp"
#include "DebugDraw.hpp"
#include "Ai.hpp"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

using namespace std;
using namespace Components;

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        GroundSensor* fixtureUserData;
        void* userData;

        //check if fixture A was the foot sensor
        userData = contact->GetFixtureA()->GetUserData();
        if (userData != NULL)
        {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts++;
        }

        //check if fixture B was the foot sensor
        userData = contact->GetFixtureB()->GetUserData();
        if (userData != NULL)
        {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts++;
        }
    }

    void EndContact(b2Contact* contact)
    {
        GroundSensor* fixtureUserData;
        void* userData;

        //check if fixture A was the foot sensor
        userData = contact->GetFixtureA()->GetUserData();
        if (userData != NULL)
        {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts--;
        }
        //check if fixture B was the foot sensor
        userData = contact->GetFixtureB()->GetUserData();
        if (userData != NULL)
        {
            fixtureUserData = (GroundSensor *)userData;
            if (!strcmp(fixtureUserData->type, "GroundSensor"))
                fixtureUserData->numFootContacts--;
        }
    }
};

double blarg(int i, double j)
{
    return i * j;
}

GinsengSandbox::GinsengSandbox(Engine *engine) : engine(engine)
{
    chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
    chai.add(chaiscript::fun(&blarg), "honk");

    double d = chai.eval<double>("honk(3, 4.75);");

    engine->EchoScreen(5, d);
    Echo(d);

    accumulator = 0.f;

    sf::Vector2u wSize = engine->window.getSize();
    gridView.reset(sf::FloatRect(wSize.x / -2.f, wSize.y / -2.f, wSize.x, wSize.y));

    PhysicsWorld physicsWorld(b2Vec2(0.f, -9.81f * 8), 1.f / 60.f, 8, 3);
    physicsWorld.World->SetContactListener(new ContactListener());
    DebugDraw* debugDraw = new DebugDraw(engine->window, 64.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    physicsWorld.World->SetDebugDraw(debugDraw);
    db.makeComponent(db.makeEntity(), physicsWorld);

    db.makeComponent(db.makeEntity(), StaticBody(physicsWorld, 0.f, -1.f, 20.f , 1.f));
    db.makeComponent(db.makeEntity(), DynamicBody(physicsWorld, 0.5f, 0.5f, 1.f , 1.f));
    db.makeComponent(db.makeEntity(), DynamicBody(physicsWorld, -0.5f, 0.5f, 1.f , 1.f));
    db.makeComponent(db.makeEntity(), DynamicBody(physicsWorld, 0.f, 1.5f, 2.f , 1.f));

    EntID player = db.makeEntity();
    DynamicBody dynamicBody = DynamicBody(physicsWorld, 4.f, 0.9f, 1.f , 1.8f);
    db.makeComponent(player, dynamicBody);
    db.makeComponent(player, FootSensor(dynamicBody.Body));
    db.makeComponent(player, AIComponent{PlayerAI(player)});
}

void GinsengSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->states.pop();
        return;
    }

    auto items = db.query<PhysicsWorld>();
    for (auto& ent : items)
    {
        PhysicsWorld& physicsWorld = std::get<1>(ent).data();
        physicsWorld.Update();
    }

    update_ais(engine, db);
}

void GinsengSandbox::draw()
{
    engine->window.setView(gridView);
    auto items = db.query<PhysicsWorld>();
    for (auto& ent : items)
    {
        PhysicsWorld& physicsWorld = std::get<1>(ent).data();
        physicsWorld.World->DrawDebugData();
    }
    engine->window.setView(engine->window.getDefaultView());
}
