#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <fstream>

namespace consts {
  double kB = 1.38064852e-23;
}

struct vec {
  vec() {
    u.x = 0;
    u.y = 0;
    u.z = 0;
  }
  vec(double xx, double yy, double zz) {
    u.x = xx;
    u.y = yy;
    u.z = zz;
  }
  union {
    struct {
      double x;
      double y;
      double z;
    };
    double a[3];
  } u;
  
  double x() {
    return u.x;
  }
  
  double y() {
    return u.y;
  }
  
  double z() {
    return u.z;
  }
  
  vec &operator /= (double s) {
    u.x /= s;
    u.y /= s;
    u.z /= s;
    return *this;
  }
};

struct At {
  double x;
  double y;
  double z;
  vec p;
};

At *at;
double *E_k;

void randInit() {
    srand( time( nullptr ) );
}

double rand01() {
    return static_cast<double>(rand())/RAND_MAX;
}

double Ekin(double T_o) {
  return -.5 * consts::kB * T_o * log(rand01());
}

#define SIGN_RAND(x) ( (rand() % 2) ? -(x) : (x) )

int main() {
  double n = 5;
  int N = n*n*n;
  double a = 0.38e-9;
  at = new At[N];
  int i = 0;
  double T_o = 100;
  double m = 1;
  
  vec b0(a, 0, 0);
  vec b1(a / 2, a * sqrt(3) / 2, 0);
  vec b2(a / 2, a * sqrt(3) / 6, a * sqrt(2./3));
  
  randInit();
  
  for (int i0 = 0; i0 < n; i0++) {
      for (int i1 = 0; i1 < n; i1++) {
	  for (int i2 = 0; i2 < n; i++, i2++) {
	    /// @todo matrix of b * i_vec
	      vec i_vec(i0 - ((n - 1)/2), i1 - ((n - 1)/2), i2 - ((n - 1)/2));
	      at[i].x  = i_vec.x() * b0.x();
	      at[i].x += i_vec.y() * b1.x();
	      at[i].x += i_vec.z() * b2.x();
	      
	      at[i].y  = i_vec.x() * b0.y();
	      at[i].y += i_vec.y() * b1.y();
	      at[i].y += i_vec.z() * b2.y();
	      
	      at[i].z  = i_vec.x() * b0.z();
	      at[i].z += i_vec.y() * b1.z();
	      at[i].z += i_vec.z() * b2.z();
	  }
      }
  }
  
  E_k = new double[N];
  
  double E_norm = 0;
  
  for (int i = 0; i < N*3; ++i) {
      E_k[i] = Ekin(T_o);
      E_norm += E_k[i];
  }
  
  /* normalizacja E_k */
  E_norm = (.5*consts::kB*T_o) / (E_norm / (3*N));
  
  vec E_p(0,0,0);
  
  for (int i = 0; i < N; i++) {
      E_k[i * 3] *= E_norm;
      E_k[i * 3 + 1] *= E_norm;
      E_k[i * 3 + 2] *= E_norm;
      
      at[i].p.u.x = SIGN_RAND(sqrt(2 * m  * E_k[i * 3]));
      at[i].p.u.y = SIGN_RAND(sqrt(2 * m  * E_k[i * 3 + 1]));
      at[i].p.u.z = SIGN_RAND(sqrt(2 * m  * E_k[i * 3 + 2]));
      
      E_p.u.x += at[i].p.u.x;
      E_p.u.y += at[i].p.u.y;
      E_p.u.z += at[i].p.u.z;
  }
  
  
  // usuniecie przemieszczania sie
  E_p.u.x /= N;
  E_p.u.y /= N;
  E_p.u.z /= N;
  
  for (int i = 0; i < N; i++) {
    at[i].p.u.x -= E_p.u.x;
    at[i].p.u.y -= E_p.u.y;
    at[i].p.u.z -= E_p.u.z;
  }
      
  
  for (int i = 0; i < N; ++i) {
      //std::cout << at[i].x << " " << at[i].y << " " << at[i].z << " " << at[i].px << " " << at[i].py << " " << at[i].pz << std::endl;
      /*std::cout << at[i].x*10e10  << " " << at[i].y*10e10  << " " << at[i].z*10e10  << " " 
                << at[i].px*10e10 << " " << at[i].py*10e10 << " " << at[i].pz*10e10 << std::endl;*/
      std::cout << at[i].p.u.x*10e10 << " " << at[i].p.u.y*10e10 << " " << at[i].p.u.z*10e10 << std::endl;
  }
      
      //delete[] at;
      //delete[] E_k;
}