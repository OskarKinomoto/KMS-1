#include "Simulation.h"

#include <iostream>
#include <cmath>

namespace {

Real pow(Real a, int n) {
    Real ret = 1;
    while(n--) {
        ret *= a;
    }
    return ret;
}

} /* namespace */

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

    /* Usunięcie przesuwania się układu */
    auto eKinSum = sumP();
    eKinSum /= -m_N;
    changeP(eKinSum);
}

void Simulation::futureToCurrent()
{
    m_current.swap(m_future);
}

void Simulation::setEKin()
{
    double t = 0;
    for (auto &at : m_current) {
        t += at.setEKin(m_T);
    }

    if (t != 0) {
        t /= (m_N * 3);
        double b = .5 * physicConstants::kB * m_T;
        double scale = sqrt(b / t);

        for (auto &at : m_current) {
            at.p() *= scale;
        }
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

Vector Simulation::sumP()
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

void Simulation::printXYZ(std::ostream &stream)
{
    for (auto &at : m_current) {
        stream << "Ar " << at.r() << std::endl;
    }
}

void Simulation::printXYZK(std::ostream &stream)
{
    for (auto &at : m_current) {
        stream << "Ar " << at.r() << " " << at.EKin() << "\n";
    }
}

void Simulation::setR(const Real &r)
{
    m_r = r;
}

void Simulation::calcFPV()
{
    m_P = 0;
    m_V = 0;

    for (auto &at : m_current) {
        at.clearF();
    }

    for (int i = 0; i < m_N; ++i) {
        auto &at = m_current.at(i);
        auto r_s = at.r().abs();

        if (r_s >= m_L) {
            m_V  += .5 * m_f * (r_s - m_L);
            at.f() += at.r() * (m_f * (m_L - r_s) / r_s);
            m_P += 1 / (4 * M_PI * m_L * m_L) * at.f().abs();
        }

        for (int j = 0; j <= i - 1; ++j) {
            auto &at2 = m_current.at(j);
            auto r_ij_vec = at.r() - at2.r();
            auto r_ij = (r_ij_vec).abs();
            auto R_r = m_r / r_ij;
            auto R_r_6 = pow(R_r, 6);
            auto R_r_12 = pow(R_r_6, 2);

            m_V += m_epsilon * (R_r_12 - 2 * R_r_6);

            auto tmp = (12 * m_epsilon * ( R_r_12 - R_r_6 )) / (r_ij * r_ij);
            auto F = r_ij_vec * tmp;
            at.f() += F;
            at2.f() -= F;
        }
    }
}

void Simulation::setL(const Real &L)
{
    m_L = L;
}

void Simulation::setF(const Real &f)
{
    m_f = f;
}

void Simulation::setEpsilon(const Real &epsilon)
{
    m_epsilon = epsilon;
}

void Simulation::step(Real tau)
{
    for (auto &at : m_current) {
        /* 18a */
        at.p() += at.f() * (.5 * tau);
        /* 18b */
        auto xx= at.p() * (tau / at.m());
        at.r() += xx;
        asm("nop");
    }
    calcFPV();

    for (auto &at : m_current) {
        /* 18c */
        at.p() += at.f() * (.5 * tau);
    }
}

Real Simulation::getT()
{
    return 2 * getAbsEKin() / (3 * m_N * physicConstants::kB);
}

Real Simulation::getV() const
{
    return m_V;
}

Real Simulation::getP() const
{
    return m_P;
}

Real Simulation::getH()
{
    Real H = 0;

    for (auto &at : m_current) {
        H += at.p().abs2();
    }
    H /= 2*(m_current.at(0).m());

    H += getV();
    return H;
}
