#include <chrono>
#include <cmath>
#include <fenv.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "Vec.h"
#include "Simulation.h"

void randInit() {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count());
}

int main(int argc, char *argv[]) {

    feenableexcept(FE_INVALID | FE_OVERFLOW);
    //randInit();

    Real a = 0.38;
    Real f = 10000;
    Real L = 2.3;
    Real epsilon = 1;
    Real tau = 1e-2;
    Real T_o = 1000;
    Real n = 3;
    int S_o, S_d, S_out, S_xyz;

    Vector b0(a, 0, 0);
    Vector b1(a / 2, a * sqrt(3) / 2, 0);
    Vector b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));

    /* load data */
    std::string parmPath = "parm.conf";
    std::string out_post_fix = "";
    if (argc == 3) {
        parmPath = argv[1];
    }

    std::ifstream conf(parmPath);
    if (conf.is_open()) {
        std::string drop;
        conf >> drop >> n;
        conf >> drop >> f;
        conf >> drop >> L;
        conf >> drop >> tau;
        conf >> drop >> T_o;
        conf >> drop >> S_o;
        conf >> drop >> S_d;
        conf >> drop >> S_out;
        conf >> drop >> S_xyz;
        conf >> drop >> a;
    }

    std::cout << n << "\t";
    std::cout << f << "\t";
    std::cout << L << "\t";
    std::cout << tau << "\t";
    std::cout << T_o << "\t";
    std::cout << S_o << "\t";
    std::cout << S_d << "\t";
    std::cout << S_out << "\t";
    std::cout << S_xyz << "\t";
    std::cout << a << "\t";
    std::cout << std::endl;



    Simulation sim(n, n, n);

    sim.setF(f);
    sim.setL(L);
    sim.setBase(b0, b1, b2);
    sim.setT(T_o);
    sim.setEpsilon(epsilon);

    sim.initPositions();
    sim.initEKin();

    sim.calcFPV();

    std::ofstream file("dat.xyz", std::ios_base::out | std::ios_base::trunc);
    std::ofstream stats("stats", std::ios_base::out | std::ios_base::trunc);
    //stats << "t\t H\t V\t T\t P" << std::endl;

    Real T_hat = 0;
    Real P_hat = 0;
    Real H_hat = 0;

    for (int s = 0; s < S_o + S_d; s++) {
        sim.step(tau);

        if (!(s % S_out)) {
            stats << s*tau << "\t" << std::scientific << std::setprecision(4) <<
                     sim.getH() << "\t" <<
                     sim.getV() << "\t" <<
                     sim.getT() << "\t" << sim.getP() << std::endl;
        }

        if (!(s % S_xyz)) {
            file << n*n*n << std::endl;
            file << "# " << s*tau << " " << sim.getT() << std::endl;
            sim.printXYZK(file);
            file << std::flush;
        }

        if (s > S_o) {
            T_hat += sim.getT();
            P_hat += sim.getP();
            H_hat += sim.getH();
        }
    }

    std::cout << "T_hat:\t" << T_hat / S_d << std::endl;
    std::cout << "P_hat:\t" << P_hat / S_d << std::endl;
    std::cout << "H_hat:\t" << H_hat / S_d << std::endl;
}
