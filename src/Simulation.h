#pragma once

#include <ostream>

#include "common.h"
#include "Atom.h"
#include "Vec.h"
#include "Matrix.h"

class Simulation {
public:
    Simulation(int nx, int ny, int nz);
    void setA(const Real &a);
    void setBase(const Matrix &base);
    void setBase(const Vector &b0, const Vector &b1, const Vector &b2);
    void setT(const Real &T);

    void initPositions();
    void initEKin();

    Vector sumP();
    void printP(std::ostream &stream);
    void printR(std::ostream &stream);
    void printRP(std::ostream &stream);
    void printXYZ(std::ostream &stream);

    void setR(const Real &r);

    void calcFPV();

    void setL(const Real &L);

    void setF(const Real &f);

    void setEpsilon(const Real &epsilon);

    void step(Real tau);

    Real getT();

private:
    Real m_nx;
    Real m_ny;
    Real m_nz;
    int m_N;

    Real m_a;
    Real m_r;
    Real m_L;
    Real m_f;
    Real m_epsilon;

    Matrix m_base;

    Real m_T;

    AtomVec m_current;
    AtomVec m_future;

    Real m_P; /// Ciśnienie układu
    Real m_V; /// Potencjał układu

private:
    void futureToCurrent();
    void saveCurrentXYZ();

    void setEKin();
    Real getAbsEKin();
    void normalizeEKin(Real s);
    void changeP(Vector p);
};
