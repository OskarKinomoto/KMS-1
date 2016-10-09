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

    Vector sumEKin();
    void printP(std::ostream &stream);
    void printR(std::ostream &stream);
    void printRP(std::ostream &stream);

private:
    Real m_nx;
    Real m_ny;
    Real m_nz;
    int m_N;

    Real m_a;

    Matrix m_base;

    Real m_T;

    AtomVec m_current;
    AtomVec m_future;

private:
    void futureToCurrent();
    void saveCurrentXYZ();

    void setEKin();
    Real getAbsEKin();
    void normalizeEKin(Real s);
    void changeP(Vector p);
};
