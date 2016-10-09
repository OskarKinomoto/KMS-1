#pragma once

#include <ostream>

#include "common.h"

#include "Vec.h"

class Matrix {
public:
    Matrix();
    Matrix(Vector v1, Vector v2, Vector v3);

    Vector operator*(const Vector &v);

private:
    Real m_data[3][3];
};

std::ostream &operator<<(std::ostream &s, const Matrix& v);
