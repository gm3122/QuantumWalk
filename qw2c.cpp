#include <cmath>

#include "qw2c.h"

// 0 down 1 up

QW2c::QW2c(int s)
{
    t = s;
    for (int i=0; i<2; i++)
        qw[i] = new Lattice<double>(t, 2*i*t);

    N = 2*t+1;
    p = new double[N];
    for (int i=0; i<N; i++)
        p[i] = 0;

    qw[1]->set(0,1);
    step = 0;
}

double **QW2c::getCoin(double a)
{
    // allocate memory
    double **c = new double*[2];
    c[0] = new double[2];
    c[1] = new double[2];

    // set coin
    c[0][0] = cos(a);
    c[0][1] = sin(a);
    c[1][0] = -sin(a);
    c[1][1] = cos(a);

    return c;
}

void QW2c::applyCoin(double **c)
{
    double temp[2];

    for (int i=-step; i<=step; i++)
    {
        for (int j=0; j<2; j++)
            temp[j] = qw[j]->at(i);
        for (int j=0; j<2; j++)
            qw[j]->set(i, c[j][0]*temp[0] + c[j][1]*temp[1]);
    }
}

void QW2c::applyDisplacement()
{
    qw[0]->moveShift(1);
    qw[1]->moveShift(-1);
    step++;
}

double *QW2c::getProbabilities()
{
    int i = 0;
    double a, b;
    for (int j=-t; j<=t; j++) {
        a = qw[0]->at(j);
        b = qw[1]->at(j);
        p[i] = a*a+b*b;
        i++;
    }

    return p;
}
