#ifndef QW2C_H
#define QW2C_H

#include "lattice.h"

class QW2c
{
public:
    QW2c(int t);

    double** getCoin(double a);
    void applyCoin(double **c);
    void applyDisplacement();

private:
    Lattice* qw[2];
    int step;
};

double **getCoin(double a);

#endif // QW2C_H
