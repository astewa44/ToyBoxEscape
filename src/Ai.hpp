//
// Created by Anthony on 9/5/2015.
//

#ifndef TOYBOXESCAPE_AI_HPP
#define TOYBOXESCAPE_AI_HPP

#include <functional>

#include "Engine.hpp"
#include "Entcom.hpp"
#include "Components.hpp"

namespace Components
{
    void update_ais(Engine* engine, DB& db);

    struct AIComponent
    {
        template <class T> AIComponent(T inputAi) { brain = inputAi; }
        std::function<void(Engine*, DB&, EntID, AIComponent &)> brain;

        void update(Engine* engine, DB& db, EntID id) { brain(engine, db, id, *this); }
    };

    struct PlayerAI
    {
        sf::Clock clock;
        sf::Time rising;

        PlayerAI(EntID me);
        void operator()(Engine* engine, DB& db, EntID me, AIComponent &my_ai);
    };
}

#endif //TOYBOXESCAPE_AI_HPP
