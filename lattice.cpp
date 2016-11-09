#include "lattice.h"

#include <complex>

template <class T>
Lattice<T>::Lattice(int t, int s)
{
    N = 2*t+1;
    l = new T[N];
    shift = s;
    init();
}

template <class T>
T Lattice<T>::at(int i)
{
    int v = i+shift;
    if (v >= 0 && v<N)
        return l[i+shift];

    return 0;
}

template <class T>
void Lattice<T>::set(int i, T value)
{
    l[i+shift] = value;
}

template <class T>
void Lattice<T>::moveShift(int i)
{
    shift += i;
}

template <class T>
void Lattice<T>::init()
{
    for (int i=0; i<N; i++)
        l[i] = 0;
}

template class Lattice<double>;
template class Lattice<std::complex<double> >;
