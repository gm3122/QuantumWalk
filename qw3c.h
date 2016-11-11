#ifndef QW3C_H
#define QW3C_H

#include <complex>

#include "lattice.h"

typedef std::complex<double> complex;

class QW3c
{
public:
    QW3c(int s);

    complex** getCoin(double a);
    void applyCoin(complex **c);
    void applyDisplacement();
    double* getProbabilities();
    double getReturnAmplitude();

private:
    Lattice<complex>* qw[3];
    double* p;
    int step;
    int t;
    int N;
};

#endif // QW3C_H
