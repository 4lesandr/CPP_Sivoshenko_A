// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector {
private:
    double x;
    double y;

public:
    Vector(double x, double y);

    void setX(double x);
    void setY(double y);

    double getX() const;
    double getY() const;

    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(double k) const;
    Vector operator/(double k) const;
    Vector operator-() const;

    double operator*(const Vector& v) const;
    double operator^(const Vector& v) const;

    double length() const;
    double angle(const Vector& v) const;
    double distance(const Vector& v) const;
    Vector perpendicular() const;
    Vector reflect(const Vector& v) const;
    Vector normalise() const;
    Vector rotate(double angle) const;
    Vector project(const Vector& v) const;
};

#endif // VECTOR_H