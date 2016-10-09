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
    Vector b0(a, 0, 0);
    Vector b1(a / 2, a * sqrt(3) / 2, 0);
    Vector b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));

    Simulation sim(5, 5, 5);
    sim.setA(a);
    sim.setBase(b0, b1, b2);
    sim.setT(100);
    sim.initPositions();
    sim.initEKin();
    sim.printRP(std::cout);
}
