#include <chrono>
#include <cmath>
#include <fenv.h>
#include <iostream>

#include "Vec.h"
#include "Simulation.h"

void randInit() {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count());
}

int main() {
    feenableexcept(FE_INVALID | FE_OVERFLOW);
    randInit();

    Real a = 0.38e-9;
    Real r = 0.38e-9; // promień atomu
    Real f = 10000; //
    Real L = 2.3e-9;
    Real epsilon = 1;

    Vector b0(a, 0, 0);
    Vector b1(a / 2, a * sqrt(3) / 2, 0);
    Vector b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));

    Simulation sim(5, 5, 5);
    sim.setA(a);
    sim.setR(r);
    sim.setF(f);
    sim.setL(L);
    sim.setBase(b0, b1, b2);
    sim.setT(100);
    sim.setEpsilon(epsilon);
    sim.initPositions();
    sim.initEKin();
    sim.calcFPV();

    sim.printRP(std::cout);
    std::cout << std::endl << std::endl;

    for (int i = 0; i < 1e4; ++i) {
        sim.step(1e-12);
        if (i < 10 || i % 100 == 0 || i == 9999)
            std::cerr << i << " " << sim.getT() << std::endl;
    }

    sim.printRP(std::cout);
    std::cerr << sim.getT() << std::endl;
}
