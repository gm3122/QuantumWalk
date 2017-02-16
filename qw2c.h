#ifndef QW2C_H
#define QW2C_H

#include <complex>

#include "lattice.h"

typedef std::complex<double> complex;

class QW2c
{
public:
    QW2c(int s);

    static double **getCoin(double a);
    void applyCoin(double **c);
    void applyDisplacement();
    double* getProbabilities();

private:
    Lattice<complex>* qw[2];
    double* p;
    int step;
    int t;
    int N;
};

#endif // QW2C_H
