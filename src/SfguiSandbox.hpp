//
// Created by Anthony on 10/6/2015.
//

#ifndef TOYBOXESCAPE_SFGUISANDBOX_HPP
#define TOYBOXESCAPE_SFGUISANDBOX_HPP

#include "Engine.hpp"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

class SfguiSandbox {
    Engine* engine = nullptr;

    sf::Clock clock;

    sfg::SFGUI m_sfgui;
    sfg::Label::Ptr m_label;
    sfg::Button::Ptr m_button;
    sfg::Box::Ptr m_box;
    sfg::Window::Ptr m_window;
    sfg::Desktop m_desktop;

public:
    SfguiSandbox(Engine* engine);
    void handleEvent(sf::Event event);
    void update();
    void draw();
    bool haltsHandleEvent() const { return true; }
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return false; }

    void OnButtonClick();
};


#endif //TOYBOXESCAPE_SFGUISANDBOX_HPP
