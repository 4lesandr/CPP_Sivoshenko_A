#include "vector.h"
#include <cmath>

Vector::Vector(double x, double y): x(x), y(y){}

double Vector::getX() const { return x; }

double Vector::getY() const { return y; }

void Vector::setX(double x){ this->x = x; }

void Vector::setY(double y){ this->y = y; }

Vector Vector::operator+(const Vector& v) const{
    return Vector(x + v.x, y + v.y);
}

Vector Vector::operator-(const Vector& v) const{
    return Vector(x - v.x, y - v.y);
}

Vector Vector::operator*(double k) const{
    return Vector(x * k, y * k);
}

Vector Vector::operator/(double k) const{
    return Vector(x / k, y / k);
}

Vector Vector::operator-() const{
    return Vector(-x, -y);
}

double Vector::operator*(const Vector& v) const{
    return x * v.x + y * v.y;
}

double Vector::operator^(const Vector& v) const{
    return x * v.y - y * v.x;
}

double Vector::length() const{
    return sqrt(x * x + y * y);
}

double Vector::angle(const Vector& v) const{
    return atan2(*this ^ v, *this * v);
}

double Vector::distance(const Vector& v) const{
    return (*this - v).length();
}

Vector Vector::normalise() const{
    return *this / length();
}

Vector Vector::rotate(double angle) const{
    return Vector(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
}

Vector Vector::project(const Vector& v) const{
    return v * (*this * v) / (v * v);
}

Vector Vector::reflect(const Vector& v) const{
    return *this - v * 2 * (*this * v) / (v * v);
}

Vector Vector::perpendicular() const{
    return Vector(-y, x);
}