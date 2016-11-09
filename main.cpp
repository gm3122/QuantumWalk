#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <cmath>

#include "lattice.h"
#include "qw2c.h"
#include "qw3c.h"

double getVariance(double *p, int t);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // open file in write mode
    QFile file("test.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 1000;

    // variables
    double *p;

//    QW2c qw(t);
//    double **c = qw.getCoin(M_PI/4);
    QW3c qw(t);
    complex **c = qw.getCoin(M_PI/4);

    for (int i=0; i<t; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        p = qw.getProbabilities();
        int k = 0;
        for (int j=-t; j<=t; j++) {
            out << i << " " << j << " " << p[k] << endl;
            k++;
        }
        out << endl;
//        out << i+1 << " " << getVariance(p,t) << endl;
    }
    out << endl;

    qDebug() << "end";

    return a.exec();
}

double getVariance(double *p, int t)
{
    double v = 0;
    int i = 0;
    for (int j=-t; j<=t; j++) {
        v += p[i]*j*j;
//        qDebug() << j << " " << p[i];
        i++;
    }
//    qDebug() << endl << v << endl << endl;
    return v;
}
