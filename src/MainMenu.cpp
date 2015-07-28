//
// Created by xxAtrain223 on 4/23/2015.
//

#include "MainMenu.hpp"
#include "PhysicsSandbox.hpp"

#include <iostream>
#include <cstdio>

using namespace std;

template <typename T, typename... Ts>
std::function<StateErasure()> maker(Ts... ts) {
	return [=](){
		return T(ts...);
	};
}

// A simple test state. Replace ASAP.
struct BlueSquare
{
    Engine* engine = nullptr;
    sf::CircleShape square = sf::CircleShape(75);
    BlueSquare(Engine* engine) : engine(engine) { square.setFillColor(sf::Color(255, 0, 0)); }
    void update() { if (engine->wasKeyPressed(sf::Keyboard::Escape)) engine->states.pop(); }
    void draw() { engine->window.draw(square); }
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return false; }
};

MainMenu::MainMenu(Engine *engine) : engine(engine) {
    if (!optionFont.loadFromFile("data/OpenSans-Regular.ttf"))
        throw "Couldn't find 'data/OpenSans-Regular.ttf'";

    items.emplace_back(make_pair("Blue Square", maker<BlueSquare>(engine)));
    items.emplace_back(make_pair("Physics Sandbox", maker<PhysicsSandbox>(engine)));
}

bool MainMenu::haltsUpdate() {
    return true;
}

bool MainMenu::haltsDraw() {
    return true;
}

void MainMenu::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape) || engine->wasKeyPressed(sf::Keyboard::BackSpace)) {
        engine->states.pop();
        return;
    }

    if (engine->wasKeyPressed(sf::Keyboard::Up) || engine->wasKeyPressed(sf::Keyboard::W))
        selected = (selected + int(items.size()) - 1) % int(items.size());

    if (engine->wasKeyPressed(sf::Keyboard::Down) || engine->wasKeyPressed(sf::Keyboard::S))
        selected = (selected + int(items.size()) + 1) % int(items.size());

    if (engine->wasKeyPressed(sf::Keyboard::Return) || engine->wasKeyPressed(sf::Keyboard::Space)) {
        engine->states.push(items[selected].second());
        return;
    }

    if (engine->wasMouseButtonPressed(sf::Mouse::Left))
    {
        sf::Text text;
        text.setFont(optionFont);
        text.setCharacterSize(40);

        for (int i = 0; i < items.size(); i++)
        {
            text.setString(items[i].first);
            text.setOrigin(text.getLocalBounds().width/2.f, 0);
            text.setPosition(engine->window.getSize().x / 2.f, 100 + text.getFont()->getLineSpacing(text.getCharacterSize()) * i);
            sf::FloatRect boundingBox;
            boundingBox = text.getGlobalBounds();
            if (boundingBox.contains(engine->getMousePosition().x, engine->getMousePosition().y))
            {
                selected = i;
                engine->states.push(items[selected].second());
                return;
            }
        }
    }
}

void MainMenu::draw() {
    sf::Text optionText;
    optionText.setFont(optionFont);
    optionText.setCharacterSize(40);

    for (int i = 0; i < items.size(); i++) {
        optionText.setString(items[i].first);
        optionText.setOrigin(sf::Vector2f(optionText.getLocalBounds().width / 2, 0));
        optionText.setPosition(engine->window.getSize().x / 2.f, 100 + optionText.getFont()->getLineSpacing(optionText.getCharacterSize()) * i);
        if (i == selected)
            optionText.setColor(sf::Color::Yellow);
        else
            optionText.setColor(sf::Color::White);
		engine->window.draw(optionText);
    }
}

