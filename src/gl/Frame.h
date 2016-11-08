#pragma once

#include <iostream>
#include <vector>
#include <Vec.h>

#include <sstream>
#include <iomanip>


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << std::fixed << a_value;
    return out.str();
}

class Frame {
public:
    Frame();

    friend std::istream &operator>>(std::istream &stream, Frame &f) {
        int n = 0;
        char buf[1024];

        stream >> n;
        f.m_at.resize(n);
        stream.getline(buf, sizeof(buf));

        std::string d;
        stream >> d >> f.time >> f.T;

        f.T_str = "T: ";
        f.T_str += to_string_with_precision(f.T, 2);
        f.T_str += "K";

        f.t_str = "t: ";
        f.t_str += to_string_with_precision(f.time, 2);
        f.t_str += "ps";

        stream.getline(buf, sizeof(buf));

        std::string a;
        Vector vec;
        double eaterD;

        for (int i = 0; i < n; ++i) {
            stream >> a;
            stream >> vec >> eaterD;
            f.m_at.at(i) = vec;
        }
        stream.getline(buf, sizeof(buf));
        return stream;
    }

    friend std::ostream &operator<<(std::ostream &s, const Frame& v) {
       for (auto at : v.m_at) {
           s << at << std::endl;
       }
       return s;
    }

    std::string T_str;
    std::string t_str;

    VectorVec &at();
    Real T;
    Real time;

private:
    VectorVec m_at;


};


