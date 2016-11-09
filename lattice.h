#ifndef LATTICE_H
#define LATTICE_H

template <class T>
class Lattice
{
public:
    Lattice(int t, int s);

    T at(int i);
    void set(int i, T value);
    void moveShift(int i);

private:
    void init();

    int N;
    int shift;
    T *l;
};

#endif // LATTICE_H
