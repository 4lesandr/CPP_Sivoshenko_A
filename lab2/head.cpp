#include <vector>
#include "vector.h"
#include "point.h"
#include "mote.h"
#include "head.h"

Head::Head(const Vector& position, const Vector& velocity)
    : Point(position, velocity), max_speed(5.0) {}

void Head::accelerate(const Vector& acceleration) {
    if ((getVelocity() + acceleration).length() < max_speed){
        setVelocity(getVelocity() + acceleration);
    }
}

std::vector<Mote> Head::emit() {
    std::vector<Mote> newMotes;
    for (int i = 0; i < 11; i++){
        Vector moteVelocity = -getVelocity() - getVelocity().perpendicular() * 0.1 * (i-5);
        newMotes.push_back(Mote(getPosition(), moteVelocity));
    }
    return newMotes;
}