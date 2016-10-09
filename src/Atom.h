#pragma once

#include <ostream>
#include <vector>

#include "common.h"
#include "Vec.h"

class Atom;

typedef std::vector<Atom> AtomVec;

class Atom {
public:
    Vector r() const;
    void setR(const Vector &r);

    Vector p() const;
    void setP(const Vector &p);
    void mulP(Real s);
    void addP(const Vector &v);

    Real EKin() const;
    void setEKin(Real T);
    void setEKin(Real EKinX, Real EKinY, Real EKinZ);

private:
    Vector m_r;
    Vector m_p;
    Real m_m = 1;
};

std::ostream &operator<<(std::ostream &s, const Atom& v);
