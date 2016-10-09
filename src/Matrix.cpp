#include "Matrix.h"

#include <cstring>

Matrix::Matrix()
{
    memset(m_data, 0, 9);
}

Matrix::Matrix(Vector v1, Vector v2, Vector v3)
{
    m_data[0][0] = v1.x();
    m_data[0][1] = v1.y();
    m_data[0][2] = v1.z();

    m_data[1][0] = v2.x();
    m_data[1][1] = v2.y();
    m_data[1][2] = v2.z();

    m_data[2][0] = v3.x();
    m_data[2][1] = v3.y();
    m_data[2][2] = v3.z();
}

Vector Matrix::operator*(const Vector &v)
{
    return Vector(
            v.x() * m_data[0][0] + v.y() * m_data[1][0] + v.z() * m_data[2][0],
            v.x() * m_data[0][1] + v.y() * m_data[1][1] + v.z() * m_data[2][1],
            v.x() * m_data[0][2] + v.y() * m_data[1][2] + v.z() * m_data[2][2]
            );
}
