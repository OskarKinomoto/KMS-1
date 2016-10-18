#include "Vec.h"

#include <cmath>

Vector::Vector() : m_x(0), m_y(0), m_z(0) {}

Vector::Vector(Real x, Real y, Real z) : m_x(x), m_y(y), m_z(z) {}

Vector &Vector::operator/=(Real s)
{
    m_x /= s;
    m_y /= s;
    m_z /= s;
    return *this;
}


Vector &Vector::operator*=(Real s)
{
    m_x *= s;
    m_y *= s;
    m_z *= s;
    return *this;
}

Vector &Vector::operator-=(const Vector &s)
{
    m_x -= s.m_x;
    m_y -= s.m_y;
    m_z -= s.m_z;
    return *this;
}

Vector &Vector::operator+=(const Vector &s)
{
    m_x += s.m_x;
    m_y += s.m_y;
    m_z += s.m_z;
    return *this;
}

Vector Vector::operator*(Real s) const
{
    return Vector(m_x*s, m_y*s, m_z*s);
}

Vector Vector::operator/(Real s) const
{
    return Vector(m_x/s, m_y/s, m_z/s);
}

Vector Vector::operator-(const Vector &v2) const
{
    return Vector(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z);
}

Vector &Vector::operator=(const Vector &v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    return *this;
}

Vector &Vector::operator()(Real x, Real y, Real z)
{
    m_x = x;
    m_y = y;
    m_z = z;
    return *this;
}

Real Vector::x() const
{
    return m_x;
}

void Vector::setX(const Real &x)
{
    m_x = x;
}

Real Vector::y() const
{
    return m_y;
}

void Vector::setY(const Real &y)
{
    m_y = y;
}

Real Vector::z() const
{
    return m_z;
}

void Vector::setZ(const Real &z)
{
    m_z = z;
}

Real Vector::abs() const
{
    return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

Real Vector::abs2() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vector Vector::one()
{
    return Vector(1,1,1);
}

std::ostream &operator<<(std::ostream &s, const Vector &v)
{
    s << v.x() << " " << v.y() << " " << v.z();
    return s;
}
