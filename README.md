# Argon Simulation
### Requirements
- CMake >= 2.8.3
- Compilator with C++14 support
- CPU with AVX support

### Running environment
Tested only under ArchLinux x86_64.

### Compilation
```
mkdir build
cd build
cmake ..
make
```

### Input file
#### File name
parm.conf

#### File format
```
n: 5
f: 10000 
L: 2.3
tau: 1e-2
T_o: 1000
S_o: 100
S_d: 2000
S_out: 20
S_xyz: 200
a: .38
```

### Output file
#### File name
dat.xyz

#### File format
.xyz file format from jmol. 
```
N – atoms
#t = time passed in simulation in ps
Ar x y z E_kin – N times
```
