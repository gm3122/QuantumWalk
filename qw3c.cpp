#include "qw3c.h"

// 0 down 1 middle 2 up

QW3c::QW3c(int s)
{
    t = s;
    for (int i=0; i<3; i++)
        qw[i] = new Lattice<complex>(t, i*t);

    N = 2*t+1;
    p = new double[N];
    for (int i=0; i<N; i++)
        p[i] = 0;

    qw[1]->set(0,1);
    step = 0;
}

complex **QW3c::getCoin(double a)
{
    // allocate memory
    complex **c = new complex*[3];
    c[0] = new complex[3];
    c[1] = new complex[3];
    c[2] = new complex[3];

    // set coin
    const complex b(0,sin(2*a)/sqrt(2));

    c[0][0] = cos(a)*cos(a);
    c[0][1] = b;
    c[0][2] = sin(a)*sin(a);
    c[1][0] = b;
    c[1][1] = cos(2*a);
    c[1][2] = -b;
    c[2][0] = c[0][2];
    c[2][1] = -b;
    c[2][2] = c[0][0];

    return c;
}

void QW3c::applyCoin(complex **c)
{
    complex temp[3];

    for (int i=-step; i<=step; i++)
    {
        for (int j=0; j<2; j++)
            temp[j] = qw[j]->at(i);
        for (int j=0; j<2; j++)
        {
            qw[0]->set(i, c[0][0]*temp[0] + c[0][1]*temp[1] + c[0][2]*temp[2]);
            qw[1]->set(i, c[1][0]*temp[0] + c[1][1]*temp[1] + c[1][2]*temp[2]);
            qw[2]->set(i, c[2][0]*temp[0] + c[2][1]*temp[1] + c[2][2]*temp[2]);
        }
    }
}

void QW3c::applyDisplacement()
{
    qw[0]->moveShift(1);
    qw[2]->moveShift(-1);
    step++;
}

double *QW3c::getProbabilities()
{
    int i = 0;
    complex a, b, c;
    for (int j=-t; j<=t; j++) {
        a = qw[0]->at(j);
        b = qw[1]->at(j);
        c = qw[2]->at(j);
        p[i] = std::norm(a)+std::norm(b)+std::norm(c);
        i++;
    }

    return p;
}
