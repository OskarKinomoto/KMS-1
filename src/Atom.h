#pragma once

#include <ostream>
#include <vector>

#include "common.h"
#include "Vec.h"

class Atom;

typedef std::vector<Atom> AtomVec;

class Atom {
public:
    Vector &r();
    void setR(const Vector &r);

    Vector &p();
    void setP(const Vector &p);
    void mulP(Real s);
    void addP(const Vector &v);

    Real EKin() const;
    void setEKin(Real T);
    void setEKin(Real EKinX, Real EKinY, Real EKinZ);

    Vector &f();

    bool operator==(const Atom &a2);

    void clearF();

    Real m() const;

private:
    Vector m_r;
    Vector m_p;
    Vector m_f;
    Real m_m = 40;
};

std::ostream &operator<<(std::ostream &s, const Atom& v);
