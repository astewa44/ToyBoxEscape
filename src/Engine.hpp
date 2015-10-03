//
// Created by Jeramy on 4/18/2015.
//

#ifndef TOYBOXESCAPE_ENGINE_HPP
#define TOYBOXESCAPE_ENGINE_HPP

#include "Echo.hpp"
#include "GameStateManager.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <unordered_map>

class Engine {
    struct KeyState {
        int last_pressed = -1;
		int last_released = -1;
    };
	
	struct MouseButtonState {
		int last_pressed = -1;
		int last_released = -1;
	};
public:
    struct MousePosition {
        int x;
        int y;
        int wheel = 0;
    };

private:
    sf::Clock clock;

    int current_tick = 0;
    std::vector<KeyState> keyboard = std::vector<KeyState>(sf::Keyboard::KeyCount);
	std::vector<MouseButtonState> mouseButtons = std::vector<MouseButtonState>(sf::Mouse::ButtonCount);
    MousePosition mousePosition;

    std::vector<sf::Event::KeyEvent> keyBuffer;
	
	sf::Font debugFont;

    std::vector<std::pair<std::string, float>> debugMessages;

    void updateDebugText(float time_step);
    void drawDebugText();

    void poll_events();
    void update();
    void draw();

    void Expand(std::stringstream& ss) {
        ss << std::endl;
    }

    template <typename Head, typename... Tail>
    void Expand(std::stringstream& ss, const Head& head, const Tail&... tail) {
        ss << head << " ";
        Expand(ss, tail...);
    }

    std::unordered_map<std::string, sf::Keyboard::Key> sfKeyMap;
    std::unordered_map<std::string, sf::Mouse::Button> sfButtonMap;

public:
    Engine();
    void go();

    template <typename... Ts>
    void EchoScreen(float expireTime, const Ts&... ts) {
        std::stringstream ss;
        Expand(ss, ts...);
        std::string str = ss.str();
        debugMessages.emplace_back(std::make_pair(str, expireTime));
    }

    bool isKeyDown(sf::Keyboard::Key key) const;
    bool isKeyUp(sf::Keyboard::Key key) const;
    bool wasKeyPressed(sf::Keyboard::Key key) const;
    bool wasKeyReleased(sf::Keyboard::Key key) const;

    bool isKeyDown(const std::string key) const;
    bool isKeyUp(const std::string key) const;
    bool wasKeyPressed(const std::string key) const;
    bool wasKeyReleased(const std::string key) const;
	
	bool isMouseButtonDown(sf::Mouse::Button button) const;
    bool isMouseButtonUp(sf::Mouse::Button button) const;
    bool wasMouseButtonPressed(sf::Mouse::Button button) const;
    bool wasMouseButtonReleased(sf::Mouse::Button button) const;

    bool isMouseButtonDown(const std::string button) const;
    bool isMouseButtonUp(const std::string button) const;
    bool wasMouseButtonPressed(const std::string button) const;
    bool wasMouseButtonReleased(const std::string button) const;

    const MousePosition& getMousePosition() const {
        return mousePosition;
    }

    const std::vector<sf::Event::KeyEvent>& getKeyBuffer() const {
        return keyBuffer;
    }

    sf::RenderWindow window;

    GameStateManager states;
};


#endif //TOYBOXESCAPE_ENGINE_HPP
