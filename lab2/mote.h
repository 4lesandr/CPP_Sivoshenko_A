#pragma once
#include "point.h"
#include "vector.h"

//пылинка создается, затухает
class Mote:public Point{
    private:
        int lifeTime;
        static inline const int maxLifeTime{5};
    public:
        Mote(const Vector& position, const Vector& velocity);
        int getLifeTime() const; 
        bool update(double dt);
};