#ifndef LATTICE_H
#define LATTICE_H


class Lattice
{
public:
    Lattice(int t, int s);

    double at(int i);
    void set(int i, double value);
    void moveShift(int i);

private:
    void init();

    int N;
    int shift;
    double *l;
};

#endif // LATTICE_H
