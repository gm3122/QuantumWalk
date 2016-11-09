#ifndef QW2C_H
#define QW2C_H

#include "lattice.h"

class QW2c
{
public:
    QW2c(int s);

    double** getCoin(double a);
    void applyCoin(double **c);
    void applyDisplacement();
    double* getProbabilities();

private:
    Lattice<double>* qw[2];
    double* p;
    int step;
    int t;
    int N;
};

#endif // QW2C_H
