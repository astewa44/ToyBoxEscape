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

    sfKeyMap["A"] = sf::Keyboard::Key::A;
    sfKeyMap["B"] = sf::Keyboard::Key::B;
    sfKeyMap["C"] = sf::Keyboard::Key::C;
    sfKeyMap["D"] = sf::Keyboard::Key::D;
    sfKeyMap["E"] = sf::Keyboard::Key::E;
    sfKeyMap["F"] = sf::Keyboard::Key::F;
    sfKeyMap["G"] = sf::Keyboard::Key::G;
    sfKeyMap["H"] = sf::Keyboard::Key::H;
    sfKeyMap["I"] = sf::Keyboard::Key::I;
    sfKeyMap["J"] = sf::Keyboard::Key::J;
    sfKeyMap["K"] = sf::Keyboard::Key::K;
    sfKeyMap["L"] = sf::Keyboard::Key::L;
    sfKeyMap["M"] = sf::Keyboard::Key::M;
    sfKeyMap["N"] = sf::Keyboard::Key::N;
    sfKeyMap["O"] = sf::Keyboard::Key::O;
    sfKeyMap["P"] = sf::Keyboard::Key::P;
    sfKeyMap["Q"] = sf::Keyboard::Key::Q;
    sfKeyMap["R"] = sf::Keyboard::Key::R;
    sfKeyMap["S"] = sf::Keyboard::Key::S;
    sfKeyMap["T"] = sf::Keyboard::Key::T;
    sfKeyMap["U"] = sf::Keyboard::Key::U;
    sfKeyMap["V"] = sf::Keyboard::Key::V;
    sfKeyMap["W"] = sf::Keyboard::Key::W;
    sfKeyMap["X"] = sf::Keyboard::Key::X;
    sfKeyMap["Y"] = sf::Keyboard::Key::Y;
    sfKeyMap["Z"] = sf::Keyboard::Key::Z;
    sfKeyMap["Num0"] = sf::Keyboard::Key::Num0;
    sfKeyMap["Num1"] = sf::Keyboard::Key::Num1;
    sfKeyMap["Num2"] = sf::Keyboard::Key::Num2;
    sfKeyMap["Num3"] = sf::Keyboard::Key::Num3;
    sfKeyMap["Num4"] = sf::Keyboard::Key::Num4;
    sfKeyMap["Num5"] = sf::Keyboard::Key::Num5;
    sfKeyMap["Num6"] = sf::Keyboard::Key::Num6;
    sfKeyMap["Num7"] = sf::Keyboard::Key::Num7;
    sfKeyMap["Num8"] = sf::Keyboard::Key::Num8;
    sfKeyMap["Num9"] = sf::Keyboard::Key::Num9;
    sfKeyMap["Escape"] = sf::Keyboard::Key::Escape;
    sfKeyMap["LControl"] = sf::Keyboard::Key::LControl;
    sfKeyMap["LShift"] = sf::Keyboard::Key::LShift;
    sfKeyMap["LAlt"] = sf::Keyboard::Key::LAlt;
    sfKeyMap["LSystem"] = sf::Keyboard::Key::LSystem;
    sfKeyMap["RControl"] = sf::Keyboard::Key::RControl;
    sfKeyMap["RShift"] = sf::Keyboard::Key::RShift;
    sfKeyMap["RAlt"] = sf::Keyboard::Key::RAlt;
    sfKeyMap["RSystem"] = sf::Keyboard::Key::RSystem;
    sfKeyMap["Menu"] = sf::Keyboard::Key::Menu;
    sfKeyMap["LBracket"] = sf::Keyboard::Key::LBracket;
    sfKeyMap["RBracket"] = sf::Keyboard::Key::RBracket;
    sfKeyMap["SemiColon"] = sf::Keyboard::Key::SemiColon;
    sfKeyMap["Comma"] = sf::Keyboard::Key::Comma;
    sfKeyMap["Period"] = sf::Keyboard::Key::Period;
    sfKeyMap["Quote"] = sf::Keyboard::Key::Quote;
    sfKeyMap["Slash"] = sf::Keyboard::Key::Slash;
    sfKeyMap["BackSlash"] = sf::Keyboard::Key::BackSlash;
    sfKeyMap["Tilde"] = sf::Keyboard::Key::Tilde;
    sfKeyMap["Equal"] = sf::Keyboard::Key::Equal;
    sfKeyMap["Dash"] = sf::Keyboard::Key::Dash;
    sfKeyMap["Space"] = sf::Keyboard::Key::Space;
    sfKeyMap["Return"] = sf::Keyboard::Key::Return;
    sfKeyMap["BackSpace"] = sf::Keyboard::Key::BackSpace;
    sfKeyMap["Tab"] = sf::Keyboard::Key::Tab;
    sfKeyMap["PageUp"] = sf::Keyboard::Key::PageUp;
    sfKeyMap["PageDown"] = sf::Keyboard::Key::PageDown;
    sfKeyMap["End"] = sf::Keyboard::Key::End;
    sfKeyMap["Home"] = sf::Keyboard::Key::Home;
    sfKeyMap["Insert"] = sf::Keyboard::Key::Insert;
    sfKeyMap["Delete"] = sf::Keyboard::Key::Delete;
    sfKeyMap["Add"] = sf::Keyboard::Key::Add;
    sfKeyMap["Subtract"] = sf::Keyboard::Key::Subtract;
    sfKeyMap["Multiply"] = sf::Keyboard::Key::Multiply;
    sfKeyMap["Divide"] = sf::Keyboard::Key::Divide;
    sfKeyMap["Left"] = sf::Keyboard::Key::Left;
    sfKeyMap["Right"] = sf::Keyboard::Key::Right;
    sfKeyMap["Up"] = sf::Keyboard::Key::Up;
    sfKeyMap["Down"] = sf::Keyboard::Key::Down;
    sfKeyMap["Numpad0"] = sf::Keyboard::Key::Numpad0;
    sfKeyMap["Numpad1"] = sf::Keyboard::Key::Numpad1;
    sfKeyMap["Numpad2"] = sf::Keyboard::Key::Numpad2;
    sfKeyMap["Numpad3"] = sf::Keyboard::Key::Numpad3;
    sfKeyMap["Numpad4"] = sf::Keyboard::Key::Numpad4;
    sfKeyMap["Numpad5"] = sf::Keyboard::Key::Numpad5;
    sfKeyMap["Numpad6"] = sf::Keyboard::Key::Numpad6;
    sfKeyMap["Numpad7"] = sf::Keyboard::Key::Numpad7;
    sfKeyMap["Numpad8"] = sf::Keyboard::Key::Numpad8;
    sfKeyMap["Numpad9"] = sf::Keyboard::Key::Numpad9;
    sfKeyMap["F1"] = sf::Keyboard::Key::F1;
    sfKeyMap["F2"] = sf::Keyboard::Key::F2;
    sfKeyMap["F3"] = sf::Keyboard::Key::F3;
    sfKeyMap["F4"] = sf::Keyboard::Key::F4;
    sfKeyMap["F5"] = sf::Keyboard::Key::F5;
    sfKeyMap["F6"] = sf::Keyboard::Key::F6;
    sfKeyMap["F7"] = sf::Keyboard::Key::F7;
    sfKeyMap["F8"] = sf::Keyboard::Key::F8;
    sfKeyMap["F9"] = sf::Keyboard::Key::F9;
    sfKeyMap["F10"] = sf::Keyboard::Key::F10;
    sfKeyMap["F11"] = sf::Keyboard::Key::F11;
    sfKeyMap["F12"] = sf::Keyboard::Key::F12;
    sfKeyMap["F13"] = sf::Keyboard::Key::F13;
    sfKeyMap["F14"] = sf::Keyboard::Key::F14;
    sfKeyMap["F15"] = sf::Keyboard::Key::F15;
    sfKeyMap["Pause"] = sf::Keyboard::Key::Pause;

    sfButtonMap["Left"] = sf::Mouse::Button::Left;
    sfButtonMap["Right"] = sf::Mouse::Button::Right;
    sfButtonMap["Middle"] = sf::Mouse::Button::Middle;
    sfButtonMap["XButton1"] = sf::Mouse::Button::XButton1;
    sfButtonMap["XButton2"] = sf::Mouse::Button::XButton2;
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
    debugText.setColor(sf::Color::Green);

    float lineSpace = debugFont.getLineSpacing(debugText.getCharacterSize());
    debugText.setOrigin(0, lineSpace);

    int windowHeight = window.getSize().y;

    for (int i = 0; i < debugMessages.size(); i++) {
        debugText.setString(debugMessages[i].first);
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

bool Engine::isKeyDown(const std::string key) const {
    return isKeyDown(sfKeyMap.at(key));
}

bool Engine::isKeyUp(const std::string key) const {
    return isKeyUp(sfKeyMap.at(key));
}

bool Engine::wasKeyPressed(const std::string key) const {
    return wasKeyPressed(sfKeyMap.at(key));
}

bool Engine::wasKeyReleased(const std::string key) const {
    return wasKeyReleased(sfKeyMap.at(key));
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

bool Engine::isMouseButtonDown(const std::string button) const {
    return isMouseButtonDown(sfButtonMap.at(button));
}

bool Engine::isMouseButtonUp(const std::string button) const {
    return isMouseButtonUp(sfButtonMap.at(button));
}

bool Engine::wasMouseButtonPressed(const std::string button) const {
    return wasMouseButtonPressed(sfButtonMap.at(button));
}

bool Engine::wasMouseButtonReleased(const std::string button) const {
    return wasMouseButtonReleased(sfButtonMap.at(button));
}
