#include <QCoreApplication>
#include <cmath>

#include "lattice.h"
#include "qw2c.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // parameters
    int t = 1000;

    QW2c qw(t);
    double **c = getCoin(M_PI/4);
    for (int i=0; i<t; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
    }

    return a.exec();
}

