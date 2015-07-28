//
// Created by Jeramy on 4/18/2015.
//

#include "Engine.hpp"
#include "MainMenu.hpp"

#include <string>
#include <algorithm>

Engine::Engine() {
    window.create(sf::VideoMode(800,600),"Toy Box Escape");
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    if (!debugFont.loadFromFile("data/cour.ttf"))
        throw "Couldn't find 'data/cour.ttf'";
}

void Engine::go() {
    states.push(MainMenu(this));
    clock.restart();
    while (window.isOpen() && !states.empty()) {
        ++current_tick;
        poll_events();
        int c = states.size();
        if (c > 0)
            update();
        if (states.size() == c)
            draw();
    }
}

void Engine::poll_events() {
    sf::Event event;
    keyBuffer.clear();
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                while (!states.empty())
                    states.pop();
                break;
            case sf::Event::KeyPressed:
                keyBuffer.push_back(event.key);
                keyboard[event.key.code].last_pressed = current_tick;
                break;
            case sf::Event::KeyReleased:
                keyboard[event.key.code].last_released = current_tick;
                break;
			case sf::Event::MouseButtonPressed:
				mouseButtons[event.mouseButton.button].last_pressed = current_tick;
				break;
			case sf::Event::MouseButtonReleased:
			    mouseButtons[event.mouseButton.button].last_released = current_tick;
                break;
            case sf::Event::MouseWheelMoved:
                mousePosition.wheel += event.mouseWheel.delta;
                break;
            case sf::Event::MouseMoved:
                mousePosition.x = event.mouseMove.x;
                mousePosition.y = event.mouseMove.y;
                break;
        }
    }
}

void Engine::updateDebugText(float time_step) {
    for (auto& m : debugMessages) {
        m.second -= time_step;
        if (m.second < 0)
            debugMessages.erase(std::remove(std::begin(debugMessages), std::end(debugMessages), m));
    }
}

void Engine::drawDebugText() {
    sf::Text debugText;

    debugText.setFont(debugFont);
    debugText.setCharacterSize(20);
    debugText.setStyle(sf::Text::Bold);

    float lineSpace = debugFont.getLineSpacing(debugText.getCharacterSize());
    int windowHeight = window.getSize().y;

    debugText.setOrigin(0, lineSpace);

    for (int i = 0; i < debugMessages.size(); i++)
    {
        debugText.setString(debugMessages[i].first);
        if (debugMessages[i].second < 1)
            debugText.setColor(sf::Color(0, 255, 0, sf::Uint8(debugMessages[i].second * 255)));
        else
            debugText.setColor(sf::Color(0, 255, 0));
        debugText.setPosition(0, windowHeight - i * lineSpace);
        window.draw(debugText);
    }
}

void Engine::update() {
    double time_step = clock.getElapsedTime().asSeconds();
	window.setTitle("Toy Box Escape-- fps: " + std::to_string(int(1 / time_step)));
    clock.restart();
    states.update();
    updateDebugText(time_step);
}

void Engine::draw() {
    window.clear();
    states.draw();
    drawDebugText();
    window.display();
}

bool Engine::isKeyDown(sf::Keyboard::Key key) const {
    return keyboard[key].last_pressed > keyboard[key].last_released;
}

bool Engine::isKeyUp(sf::Keyboard::Key key) const {
    return !isKeyDown(key);
}

bool Engine::wasKeyPressed(sf::Keyboard::Key key) const {
    return keyboard[key].last_pressed == current_tick;
}

bool Engine::wasKeyReleased(sf::Keyboard::Key key) const {
    return keyboard[key].last_released == current_tick;
}

bool Engine::isMouseButtonDown(sf::Mouse::Button button) const {
    return mouseButtons[button].last_pressed > mouseButtons[button].last_released;
}

bool Engine::isMouseButtonUp(sf::Mouse::Button button) const {
    return !isMouseButtonDown(button);
}

bool Engine::wasMouseButtonPressed(sf::Mouse::Button button) const {
    return mouseButtons[button].last_pressed == current_tick;
}

bool Engine::wasMouseButtonReleased(sf::Mouse::Button button) const {
    return mouseButtons[button].last_released == current_tick;
}

