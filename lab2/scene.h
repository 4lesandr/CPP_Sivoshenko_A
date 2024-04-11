#pragma once

#include <vector>
#include "mote.h"
#include "head.h"


enum class Symbol;

class Scene{
    private:
        std::vector<Mote> motes;
        Head head;
    public:
        Scene(const Head& head);
        void update(double dt);
        void draw();
        bool processInput();
};
