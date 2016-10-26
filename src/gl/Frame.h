#pragma once

#include <iostream>
#include <vector>
#include <Vec.h>

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
        stream >> d >> d >> f.T;

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

    VectorVec &at();
    Real T;

private:
    VectorVec m_at;


};


