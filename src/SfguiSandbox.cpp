//
// Created by Anthony on 10/6/2015.
//

#include "SfguiSandbox.hpp"

SfguiSandbox::SfguiSandbox(Engine *engine) : engine(engine) {
    m_label = sfg::Label::Create("Hello World!");

    m_button = sfg::Button::Create("Greet SFGUI!");
    m_button->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&SfguiSandbox::OnButtonClick, this));

    m_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    m_box->Pack( m_label );
    m_box->Pack( m_button, false );

    m_window = sfg::Window::Create();
    m_window->SetTitle( "Hello world!" );
    m_window->Add( m_box );

    m_desktop.Add(m_window);
}

void SfguiSandbox::update() {
    if (engine->wasKeyPressed(sf::Keyboard::Escape) || engine->wasKeyPressed(sf::Keyboard::BackSpace)) {
        engine->states.pop();
        return;
    }

    m_desktop.Update(clock.restart().asSeconds());
}

void SfguiSandbox::draw() {
    m_sfgui.Display(engine->window);
}

void SfguiSandbox::OnButtonClick() {
    m_label->SetText("Hello SFGUI, pleased to meet you!");
}

void SfguiSandbox::handleEvent(sf::Event event) {
    m_desktop.HandleEvent(event);
}
