#include <chrono>
#include <cmath>
#include <fenv.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <signal.h>

#include <sys/ioctl.h>
#include <unistd.h>

#include "Vec.h"
#include "Simulation.h"

/*
 *
 */

void randInit() {
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count());
}

void bash_show_cursor() {
    std::cout << "\033[?25h" << std::flush;
}

void bash_hide_cursor() {
    std::cout << "\033[?25l" << std::flush;
}

void bash_clear_line() {
    std::cout << "\33[2K\r" << std::flush;
}

int bash_columns() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

void bash_progress_bar(int s, int max) {
    float per = static_cast<float>(s) / max;
    auto width = bash_columns();
    auto progress_width = width - 2 - 7;
    int done = progress_width * per;
    int to_do = progress_width - done;

    bash_clear_line();

    std::cout << "[";
    for (int i = 0; i < done; ++i) {
        std::cout << "=";
    }
    for (int i = 0; i < to_do; ++i) {
        std::cout << "-";
    }

    int b = per * 100;
    int l = (per * 100 - b) * 100;

    std::cout << "] ";

    if (!to_do) {
        std::cout << "100.0%" << std::flush;
    } else {
        std::cout << std::setfill('0') << std::setw(2)
                  << b << "." << std::setfill('0') << std::setw(2)
                  << l << "%" << std::flush;
    }
}

void handle_signal(int) {
    bash_show_cursor();
    std::cout << "\b\b  " << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    sa.sa_handler = &handle_signal;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);

    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    feenableexcept(FE_INVALID | FE_OVERFLOW);
    randInit();

    Real a = 0.38;
    Real f = 10000;
    Real L = 2.3;
    Real epsilon = 1;
    Real tau = 1e-2;
    Real T_o = 1000;
    Real n = 3;
    bool run = true;
    int S_o;

    int xyz_count = 200;
    int stats_count = 1000;
    Real time = 2;

    /* load data */
    std::string parmPath = "parm.conf";
    std::string out_post_fix = "";
    if (argc == 3) {
        parmPath = argv[1];
        out_post_fix = "-";
        out_post_fix += argv[2];
    }

    std::ifstream conf(parmPath);
    if (conf.is_open()) {
        std::string drop;
        int int_drop;
        conf >> drop >> n;
        conf >> drop >> f;
        conf >> drop >> L;
        conf >> drop >> tau;
        conf >> drop >> T_o;
        conf >> drop >> S_o;
        conf >> drop >> int_drop;
        conf >> drop >> int_drop;
        conf >> drop >> int_drop;
        conf >> drop >> a;
        conf >> drop >> run;
        conf >> drop >> time;
        conf >> drop >> xyz_count;
        conf >> drop >> stats_count;
    }

    if (false) {
        std::cout << n << "\t";
        std::cout << f << "\t";
        std::cout << L << "\t";
        std::cout << tau << "\t";
        std::cout << T_o << "\t";
        std::cout << S_o << "\t";
        std::cout << a << "\t";
        std::cout << std::endl;
    }

    Vector b0(a, 0, 0);
    Vector b1(a / 2, a * sqrt(3) / 2, 0);
    Vector b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));

    Simulation sim(n, n, n);

    sim.setF(f);
    sim.setL(L);
    sim.setBase(b0, b1, b2);
    sim.setT(T_o);
    sim.setEpsilon(epsilon);

    sim.initPositions();
    sim.initEKin();

    sim.calcFPV();

    Real T_hat = 0;
    Real P_hat = 0;
    Real H_hat = 0;

    int steps = ceil(time / tau);

    int xyz_mod = steps / xyz_count;
    if (xyz_mod < 1) xyz_mod = 1;

    int stats_mod = steps / stats_count;
    if (stats_mod < 1) stats_mod = 1;


    if (run) {
        std::ofstream file("dat" + out_post_fix + ".xyz", std::ios_base::out | std::ios_base::trunc);
        std::ofstream stats("stats" + out_post_fix, std::ios_base::out | std::ios_base::trunc);
        stats << "t\t H\t V\t T\t P" << std::endl;

        bash_hide_cursor();

        for (int s = 0; s < steps; s++) {
            sim.step(tau);

            if (!(s % stats_mod)) {
                stats << s*tau << "\t" << std::scientific << std::setprecision(4) <<
                         sim.getH() << "\t" <<
                         sim.getV() << "\t" <<
                         sim.getT() << "\t" <<
                         sim.getP() << "\t" <<
                         sim.getEkin() <<
                         std::endl;
            }

            if (!(s % 10)) {
                bash_progress_bar(s, steps);
            }

            if (!(s % xyz_mod)) {
                file << n*n*n << std::endl;
                file << "# " << s*tau << " " << sim.getT() << " " << sim.getP() << " " << sim.getH() << " " << sim.getEkin() << std::endl;
                sim.printXYZK(file);
                file << std::flush;
            }

            if (s > S_o) {
                T_hat += sim.getT();
                P_hat += sim.getP();
                H_hat += sim.getH();
            }
        }
        bash_progress_bar(1, 1);

        std::cout << std::endl;

        bash_show_cursor();

    } else {
        std::cout << a << "\t" << sim.getV() << std::endl;
    }

    if (steps != S_o) {
        std::cout << "T_hat:\t" << T_hat / (steps-S_o) << std::endl;
        std::cout << "P_hat:\t" << P_hat / (steps-S_o) << std::endl;
        std::cout << "H_hat:\t" << H_hat / (steps-S_o) << std::endl;
        std::cerr << T_hat / (steps-S_o) << "\t" << P_hat / (steps-S_o) <<
                     "\t" << H_hat / (steps-S_o) << std::endl;
    }
}
