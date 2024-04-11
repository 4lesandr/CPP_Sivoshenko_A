#pragma once
#include <vector>
#include "vector.h"
#include "point.h"
#include "mote.h"


class Head: public Point{
    private:
        double const max_speed;
    public:
        Head(const Vector& position, const Vector& velocity);
        void accelerate(const Vector& acceleration);
        std::vector<Mote> emit();
};



