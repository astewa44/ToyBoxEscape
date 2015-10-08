//
// Created by xxAtrain223 on 4/23/2015.
//

#ifndef TOYBOXESCAPE_MAINMENU_HPP
#define TOYBOXESCAPE_MAINMENU_HPP

#include "Engine.hpp"

#include <sfml/Graphics.hpp>

#include <vector>
#include <utility>

class MainMenu {
    Engine *engine = nullptr;

    sf::Font optionFont;

    std::vector<std::pair<std::string, std::function<StateErasure()>>> items;
    int selected = 0;

public:
	MainMenu() = default;
    MainMenu(Engine *engine);
    bool haltsHandleEvent() { return true; }
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
    void handleEvent(sf::Event event) {}
    void update();
    void draw();
};

#endif //TOYBOXESCAPE_MAINMENU_H
