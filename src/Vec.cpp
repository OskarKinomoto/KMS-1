#include "Vec.h"

#include <cmath>

Vector::Vector() : m_x(0), m_y(0), m_z(0) {}

Vector::Vector(Real x, Real y, Real z) : m_x(x), m_y(y), m_z(z) {}

Vector::Vector(__m256d d) : m_d(d) {}

Vector &Vector::operator/=(Real s)
{
    __m256d scalar = _mm256_set1_pd(s);
    m_d = _mm256_div_pd(m_d, scalar);
    return *this;
}


Vector &Vector::operator*=(Real s)
{
    auto scalar = _mm256_set1_pd(s);
    m_d = _mm256_mul_pd(m_d, scalar);
    return *this;
}

Vector &Vector::operator-=(const Vector &s)
{
    m_d = _mm256_sub_pd(m_d, s.m_d);
    return *this;
}

Vector &Vector::operator+=(const Vector &s)
{
    m_d = _mm256_add_pd(m_d, s.m_d);
    return *this;
}

Vector Vector::operator*(Real s) const
{
    auto scalar = _mm256_set1_pd(s);
    return Vector(_mm256_mul_pd(m_d, scalar));
}

Vector Vector::operator/(Real s) const
{
    auto scalar = _mm256_set1_pd(s);
    return Vector(_mm256_div_pd(m_d, scalar));
}

Vector Vector::operator-(const Vector &v2) const
{
    return Vector(_mm256_sub_pd(m_d, v2.m_d));
}

Vector &Vector::operator=(const Vector &v)
{
    m_d = v.m_d;
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
    return sqrt(abs2());
}

Real Vector::abs2() const
{
    auto tmp = _mm256_mul_pd(m_d, m_d);
    return tmp[2] + tmp[1] + tmp[0];
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
