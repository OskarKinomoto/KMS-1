#include "Simulation.h"

namespace {

Real TToEKin(Real T) {
    return .5 * physicConstants::kB * T;
}

}

Simulation::Simulation(int nx, int ny, int nz) : m_nx(nx), m_ny(ny), m_nz(nz)
{
    m_N = nx * ny * nz;

    m_current.resize(m_N);
    m_future.resize(m_N);
}

void Simulation::setA(const Real &a)
{
    m_a = a;
}

void Simulation::setBase(const Matrix &base)
{
    m_base = base;
}

void Simulation::setBase(const Vector &b0, const Vector &b1, const Vector &b2)
{
    m_base = Matrix(b0, b1, b2);
}

void Simulation::setT(const Real &T)
{
    m_T = T;
}

void Simulation::initPositions()
{
    int i = 0;

    for (int i0 = 0; i0 < m_nx; ++i0) {
        for (int i1 = 0; i1 < m_ny; ++i1) {
            for (int i2 = 0; i2 < m_nz; ++i2, ++i) {
                Vector vec(i0 - ((m_nx - 1)/2),
                           i1 - ((m_ny - 1)/2),
                           i2 - ((m_nz - 1)/2));
                m_current.at(i).setR(m_base * vec);
            }
        }
    }
}

void Simulation::initEKin()
{
    setEKin();

    /* Normalizacja E_kin względem temperatury */
    auto eKinAbsSum = getAbsEKin();

    auto eKinPerAt = eKinAbsSum / m_N;

    normalizeEKin(TToEKin(m_T) / eKinPerAt);

    /* Usunięcie przesuwania się układu */
    auto eKinSum = sumEKin();
    eKinSum /= -m_N;
    changeP(eKinSum);
}

void Simulation::futureToCurrent()
{
    m_current.swap(m_future);
}

void Simulation::setEKin()
{
    for (auto &at : m_current) {
        at.setEKin(m_T);
    }
}

Real Simulation::getAbsEKin()
{
    Real ret = 0;
    for (auto &at : m_current) {
        ret += at.EKin();
    }
    return ret;
}

void Simulation::normalizeEKin(Real s)
{
    for (auto &at : m_current) {
        at.mulP(s);
    }
}

void Simulation::changeP(Vector p)
{
    for (auto &at : m_current) {
        at.addP(p);
    }
}

Vector Simulation::sumEKin()
{
    Vector ret(0,0,0);
    for (auto &at : m_current) {
        ret += at.p();
    }
    return ret;
}

void Simulation::printP(std::ostream &stream)
{
    for (auto &at : m_current) {
        stream << at.p() << std::endl;
    }
}

void Simulation::printR(std::ostream &stream)
{
    for (auto &at : m_current) {
        stream << at.r() << std::endl;
    }
}

void Simulation::printRP(std::ostream &stream)
{
    for (auto &at : m_current) {
        stream << at.r() << " " << at.p() << std::endl;
    }
}
