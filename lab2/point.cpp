#include "vector.h"
#include "point.h"

#define MAX_WIDTH 206
#define MAX_HEIGHT 65

Point::Point(const Vector& position, const Vector& velocity)
    : position(position), velocity(velocity) {}

void Point::setPosition(const Vector& position) { this->position = position; }

void Point::setVelocity(const Vector& velocity) { this->velocity = velocity; }

Vector Point::getPosition() const { return position; }

Vector Point::getVelocity() const { return velocity; }

void Point::move(double dt){
    if (position.getX() < 0){
        position.setX(-position.getX());
        velocity = velocity.reflect({1, 0});
    }
    if (position.getX() >= MAX_WIDTH){
        position.setX(2 * MAX_WIDTH - position.getX());
        velocity = velocity.reflect({-1, 0});
    }
    if (position.getY() < 0){
        position.setY(-position.getY());
        velocity = velocity.reflect({0, 1});
    }
    if (position.getY() >= MAX_HEIGHT){
        position.setY(2 * MAX_HEIGHT - position.getY());
        velocity = velocity.reflect({0, -1});
    }
    
    double aspectRatio = (24.0 / 11.0) * (static_cast<double>(MAX_HEIGHT) / MAX_WIDTH);
    Vector scaledVelocity = {velocity.getX() / aspectRatio, velocity.getY()};

    position = position + scaledVelocity * dt;
}