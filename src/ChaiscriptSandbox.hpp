//
// Created by Anthony on 11/27/2015.
//

#ifndef TOYBOXESCAPE_CHAISCRIPTSANDBOX_HPP
#define TOYBOXESCAPE_CHAISCRIPTSANDBOX_HPP

#include "Engine.hpp"
#include "Entcom.hpp"
#include "Components.hpp"
#include "ChaiGameObject.hpp"
#include <memory>

class ChaiscriptSandbox {
private:
    Engine* engine = nullptr;

    DB db;

    std::shared_ptr<ChaiGameObject> go;

public:
    ChaiscriptSandbox(Engine* engine);
    void update();
    void draw();
    bool haltsUpdate() const { return true; }
    bool haltsDraw() const { return true; }
};


#endif //TOYBOXESCAPE_CHAISCRIPTSANDBOX_HPP
