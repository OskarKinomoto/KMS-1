#include "Atom.h"

#include <cmath>

namespace {

Real EKinToP(Real E_kin, Real m) {
    Real res = sqrt(2 * m * E_kin);
    return res * (rand() & 1 ? -1 : 1);
}

double rand01() {
    return static_cast<double>(rand())/RAND_MAX;
}

Real TToEKin(Real T) {
    return -.5 * physicConstants::kB * T * log(rand01());
}

}

Vector Atom::r() const
{
    return m_r;
}

void Atom::setR(const Vector &r)
{
    m_r = r;
}

Vector Atom::p() const
{
    return m_p;
}

void Atom::setP(const Vector &p)
{
    m_p = p;
}

void Atom::mulP(Real s)
{
    m_p *= s;
}

void Atom::addP(const Vector &v)
{
    m_p += v;
}

Real Atom::EKin() const
{
    return m_p.abs2() / ( 2 * m_m);
}

void Atom::setEKin(Real T)
{
    setEKin(TToEKin(T), TToEKin(T), TToEKin(T));
}

void Atom::setEKin(Real EKinX, Real EKinY, Real EKinZ)
{
    m_p.setX(EKinToP(EKinX, m_m));
    m_p.setY(EKinToP(EKinY, m_m));
    m_p.setZ(EKinToP(EKinZ, m_m));
}
