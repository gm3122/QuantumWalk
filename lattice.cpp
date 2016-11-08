#include "lattice.h"

Lattice::Lattice(int t, int s)
{
    N = 2*t+1;
    l = new double[N];
    shift = s;
    init();
}

double Lattice::at(int i)
{
    return l[i+shift];
}

void Lattice::set(int i, double value)
{
    l[i+shift] = value;
}

void Lattice::moveShift(int i)
{
    shift += i;
}

void Lattice::init()
{
    for (int i=0; i<N; i++)
        l[i] = 0;
}
