#pragma once

#include <ostream>

// AVX
#include <immintrin.h>

#include "common.h"

class Vector {
public:
    Vector();
    Vector(Real x, Real y, Real z);
    Vector(__m256d d);

    Vector &operator/=(Real s);
    Vector &operator*=(Real s);
    Vector &operator-=(const Vector &s);
    Vector &operator+=(const Vector &s);

    Vector operator*(Real s) const;
    Vector operator/(Real s) const;
    Vector operator-(const Vector &v2) const;

    Vector &operator=(const Vector &v);

    Real x() const;
    void setX(const Real &x);

    Real y() const;
    void setY(const Real &y);

    Real z() const;
    void setZ(const Real &z);

    Real abs() const;
    Real abs2() const;

    static Vector one();
private:
    union {
        __m256d m_d;
        struct {
            Real m_x;
            Real m_y;
            Real m_z;
            Real m_0;
        };
    };
};

std::ostream &operator<<(std::ostream &s, const Vector& v);
