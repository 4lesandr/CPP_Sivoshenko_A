// Point.h

#ifndef POINT_H
#define POINT_H

#include "vector.h"


#define MAX_WIDTH 206
#define MAX_HEIGHT 65

class Point {
private:
    Vector position;
    Vector velocity;

public:
    Point(const Vector& position, const Vector& velocity);

    void setPosition(const Vector& position);
    void setVelocity(const Vector& velocity);
    void move(double dt);

    Vector getPosition() const;
    Vector getVelocity() const;
};

#endif // POINT_H