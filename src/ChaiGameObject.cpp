//
// Created by Anthony on 11/26/2015.
//

#include "ChaiGameObject.hpp"
using namespace std;

ChaiGameObject::ChaiGameObject(std::shared_ptr <chaiscript::ChaiScript> chai, const std::string &scriptFile) : chai(chai) {
    static int i = 0;
    i++;

    isValid = true;

    obj_id = "obj_" + to_string(i);

    int beginPos = scriptFile.rfind("/");
    if (beginPos == string::npos)
        beginPos = 0;
    else
        beginPos++;

    int endPos = scriptFile.rfind(".chai");
    if (endPos == string::npos) {
        cerr << "Invalid chaiscript file extension: " << scriptFile << endl;
        throw "Invalid chaiscript file extension: " + scriptFile;
    }

    name = scriptFile.substr(beginPos, endPos - beginPos);

    try {
        chai->use(scriptFile);
    }
    catch (const std::exception& e) {
        isValid = false;
        cerr << "Invalid file: " << scriptFile << endl << e.what() << endl;
        return;
    }

    try {
        chai->eval("var " + obj_id + " = " + name + "();");
    }
    catch (const std::exception& e) {
        isValid = false;
        cerr << "Error in " << name << " constructor." << endl << e.what() << endl;
        return;
    }
}

void ChaiGameObject::update() {
    if (isValid) {
        try {
            chai->eval(obj_id + ".update()");
        }
        catch (const std::exception& e) {
            cerr << "Error in " << name << " update function." << endl << e.what() << endl;
            isValid = false;
        }
    }
}

void ChaiGameObject::draw() {
    if (isValid) {
        try {
            chai->eval(obj_id + ".draw()");
        }
        catch (const std::exception& e) {
            cerr << "Error in " << name << " draw function." << endl << e.what() << endl;
            isValid = false;
        }
    }
}
