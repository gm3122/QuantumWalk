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
//    double *p;
    double amp[t+1];


//    QW2c qw(t);
    QW3c qw(t);
    complex **c = qw.getCoin(M_PI/4);
    amp[0] = qw.getReturnAmplitude();
//    complex **c;
//    complex **c1 = qw.getCoin(M_PI/3);
//    complex **c2 = qw.getCoin(M_PI/5);
//    qsrand(0);

    for (int i=0; i<t; i++)
    {
//        c = i%2 ? c1 : c2;
//        c = qw.getCoin(M_PI*qrand()/2/RAND_MAX);
        qw.applyCoin(c);
//        delete c;
        qw.applyDisplacement();
//        p = qw.getProbabilities();
        amp[i+1] = qw.getReturnAmplitude();
//        int k = 0;
//        for (int j=-t; j<=t; j++) {
//            out << i << " " << j << " " << p[k] << endl;
//            k++;
//        }
//        out << endl;
//        out << i+1 << " " << getVariance(p,t) << endl;
    }
//    out << endl;
//    int k = 0;
//    for (int j=-t; j<=t; j++) {
//        out << j << " " << p[k] << endl;
//        k++;
//    }

    for (int i=0; i<=t; i++) {
        for (int j=0; j<=t; j++) {
            out << i << " " << j << " " << amp[std::abs(j-i)] << endl;
        }
        out << endl;
    }

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
