//
// Created by Anthony on 11/26/2015.
//

#ifndef TOYBOXESCAPE_CHAIGAMEOBJECT_HPP
#define TOYBOXESCAPE_CHAIGAMEOBJECT_HPP

#include <functional>
#include <string>
#include <iostream>
#include <chaiscript/chaiscript.hpp>

class ChaiGameObject {
    std::shared_ptr<chaiscript::ChaiScript> chai;
    std::string obj_id;
    std::string name;
    bool isValid;

public:
    ChaiGameObject(std::shared_ptr<chaiscript::ChaiScript> chai, const std::string& scriptFile);

    void update();
    void draw();
};


#endif //TOYBOXESCAPE_CHAIGAMEOBJECT_HPP
