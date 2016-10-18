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
    //randInit();

    Real a = 0.38;
    Real r = 0.38; // promień atomu
    Real f = 10000; //
    Real L = 2.3;
    Real epsilon = 1;
    Real tau = 1e-2;
    Real time = 100;
    Real n = 3;

    Vector b0(a, 0, 0);
    Vector b1(a / 2, a * sqrt(3) / 2, 0);
    Vector b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));

    Simulation sim(n, n, n);
    sim.setA(a);
    sim.setR(r);
    sim.setF(f);
    sim.setL(L);
    sim.setBase(b0, b1, b2);
    sim.setT(1000);
    sim.setEpsilon(epsilon);

    sim.initPositions();
    sim.initEKin();

    sim.calcFPV();


    int s = 0;
    for (Real i = 0; i < time; i+= tau, s++) {
        sim.step(tau);
        if (s % 100 == 0) {
            std::cout << n*n*n << std::endl;
            std::cout << "tmp" << std::endl;
            sim.printXYZ(std::cout);
            //std::cout << std::endl << std::endl;
            //std::cout << std::endl;
        }
        //std::cout << std::endl << std::endl;
    }

    //sim.printXYZ(std::cout);
    std::cerr << sim.getT() << std::endl;
}
