#include "point.h"
#include "vector.h"
#include "mote.h"

#define MAX_WIDTH 206
#define MAX_HEIGHT 65

Mote::Mote(const Vector& position, const Vector& velocity)
    : Point(position, velocity), lifeTime(0) {}

int Mote::getLifeTime() const { return lifeTime; }

bool Mote::update(double dt){
    double aspectRatio = (24.0 / 11.0) * (static_cast<double>(MAX_HEIGHT) / MAX_WIDTH);
    Vector scaledVelocity = {getVelocity().getX() / aspectRatio, getVelocity().getY()};

    setPosition(getPosition() + scaledVelocity * dt);
    lifeTime++;
    if (lifeTime < maxLifeTime){
        return true;
    }
    else return false;
}