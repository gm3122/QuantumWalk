#include <cmath>

#include "qw2c.h"

using namespace std;

// 0 down 1 up

QW2c::QW2c(int t)
{
    for (int i=0; i<2; i++)
        qw[i] = new Lattice(t, 2*i*t);

    qw[1]->set(0,1/sqrt(2));
    step = 0;
}

void QW2c::applyCoin(double **c)
{
    double temp[2];

    for (int i=-step; i<=step; i++)
    {
        for (int j=0; j<2; j++)
            temp[j] = qw[j]->at(i);
        for (int j=0; j<2; j++)
        {
            qw[0]->set(i, c[0][0]*temp[0] + c[0][1]*temp[1]);
            qw[1]->set(i, c[0][0]*temp[0] + c[0][1]*temp[1]);
        }
    }
}

void QW2c::applyDisplacement()
{
    qw[0]->moveShift(1);
    qw[1]->moveShift(-1);
    step++;
}

double **getCoin(double a)
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
