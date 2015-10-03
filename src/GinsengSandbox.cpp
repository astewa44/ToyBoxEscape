//
// Created by Anthony on 8/31/2015.
//

#include "GinsengSandbox.hpp"
#include "DebugDraw.hpp"
#include "Ai.hpp"
#include "Components.hpp"

#include <chaiscript/utility/utility.hpp>

using namespace std;
using namespace Components;
using namespace chaiscript;

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

GinsengSandbox::GinsengSandbox(Engine *engine) : engine(engine), chai(std::make_shared<ChaiScript>(Std_Lib::library()))
{
    ModulePtr sensorModule(new Module());

    utility::add_class<FootSensor>(*sensorModule,
        "FootSensor",
        {  },
        { {fun(&FootSensor::OnGround), "OnGround"} }
    );

    chai->add(sensorModule);

    ModulePtr engineModule(new Module());

    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::isKeyDown), engine), "isKeyDown");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::isKeyUp), engine), "isKeyUp");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::wasKeyPressed), engine), "wasKeyPressed");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::wasKeyReleased), engine), "wasKeyReleased");

    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::isMouseButtonDown), engine), "isMouseButtonDown");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::isMouseButtonUp), engine), "isMouseButtonUp");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::wasMouseButtonPressed), engine), "wasMouseButtonPressed");
    engineModule->add(fun(static_cast<bool (Engine::*)(const std::string) const>(&Engine::wasMouseButtonReleased), engine), "wasMouseButtonReleased");

    engineModule->add(user_type<Engine::MousePosition>(), "MousePosition");
    engineModule->add(fun(&Engine::MousePosition::x), "x");
    engineModule->add(fun(&Engine::MousePosition::y), "y");
    engineModule->add(fun(&Engine::MousePosition::wheel), "wheel");

    engineModule->add(fun(&Engine::getMousePosition, engine), "getMousePosition");

    chai->add(engineModule);

    chai->add(var(engine), "engine");

    accumulator = 0.f;

    sf::Vector2u wSize = engine->window.getSize();
    gridView.reset(sf::FloatRect(wSize.x / -2.f, wSize.y / -2.f, wSize.x, wSize.y));

    PhysicsWorld physicsWorld(b2Vec2(0.f, -9.81f * 8), 1.f / 60.f, 8, 3);
    physicsWorld.World->SetContactListener(new ContactListener());
    DebugDraw* debugDraw = new DebugDraw(engine->window, 64.f);
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    physicsWorld.World->SetDebugDraw(debugDraw);
    db.makeComponent(db.makeEntity(), physicsWorld);

    loadLevel("data/scripts/level/test.json");

    EntID player = db.makeEntity();
    db.makeComponent(player, DynamicBody(physicsWorld, 4.f, 0.9f, 1.f , 1.8f));
    db.makeComponent(player, FootSensor(player.get<DynamicBody>().data().Body));
    db.makeComponent(player, AIComponent{PlayerAI(player)});
    chai->add(var(player.get<FootSensor>().data()), "foot");
}

void GinsengSandbox::update()
{
    if (engine->wasKeyPressed(sf::Keyboard::Escape))
    {
        engine->states.pop();
        return;
    }

    for (auto& ent : db.query<PhysicsWorld>())
    {
        PhysicsWorld& physicsWorld = std::get<1>(ent).data();
        physicsWorld.Update();
    }

    chai->eval_file("data/scripts/test.chai");

    update_ais(engine, db);
}

void GinsengSandbox::draw()
{
    engine->window.setView(gridView);
    for (auto& ent : db.query<PhysicsWorld>())
    {
        PhysicsWorld& physicsWorld = std::get<1>(ent).data();
        physicsWorld.World->DrawDebugData();
    }
    engine->window.setView(engine->window.getDefaultView());
}

void GinsengSandbox::loadLevel(string fname)
{
    Json::Value json;
    std::ifstream file(fname.c_str());
    file >> json;

    load(json);
}

void GinsengSandbox::load(Json::Value json)
{
    for (auto& ent : db.query<PhysicsWorld>())
    {
        PhysicsWorld& physicsWorld = std::get<1>(ent).data();

        for (auto& body : json["staticBodies"]) {
            db.makeComponent(db.makeEntity(), StaticBody(physicsWorld,
                                                         body["center"]["x"].asFloat(),
                                                         body["center"]["y"].asFloat(),
                                                         body["size"]["w"].asFloat(),
                                                         body["size"]["h"].asFloat()));
        }

        for (auto& body : json["dynamicBodies"]) {
            db.makeComponent(db.makeEntity(), DynamicBody(physicsWorld,
                                                          body["center"]["x"].asFloat(),
                                                          body["center"]["y"].asFloat(),
                                                          body["size"]["w"].asFloat(),
                                                          body["size"]["h"].asFloat()));
        }
    }
}