#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <cmath>

#include "lattice.h"
#include "qw2c.h"
#include "qw3c.h"

void qw2(int t, QTextStream &out);
void qw3(int t, QTextStream &out);
double getVariance(double *p, int t);
bool* fibo(int t);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // open file in write mode
    QFile file("test.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 1000;

    qsrand(0);
//    qw2(t, out);
    qw3(t, out);

    qDebug() << "end";

    return a.exec();
}

void qw2(int t, QTextStream &out)
{
    double *p;

    QW2c qw(t);
    double **c = qw.getCoin(M_PI/4);

    for (int i=0; i<t; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        p = qw.getProbabilities();

        // probabilities
        int k = 0;
        for (int j=-t; j<=t; j++) {
            out << i+1 << " " << j << " " << p[k] << endl;
            k++;
        }
        out << endl;
        // variance
//        out << i+1 << " " << getVariance(p,t) << endl;
    }
}

void qw3(int t, QTextStream &out)
{
    double amp[t+1];

    QW3c qw(t);
    complex **c = qw.getCoin(M_PI/4);

    amp[0] = qw.getReturnAmplitude();

    for (int i=0; i<t; i++)
    {
        // c = qw.getCoin(M_PI*qrand()/2/RAND_MAX);
        qw.applyCoin(c);
        // delete c;
        qw.applyDisplacement();
        amp[i+1] = qw.getReturnAmplitude();
    }

    for (int i=0; i<=t; i++) {
        for (int j=0; j<=t; j++) {
            out << i << " " << j << " " << amp[std::abs(j-i)] << endl;
        }
        out << endl;
    }
}

double getVariance(double *p, int t)
{
    double v = 0;
    int i = 0;
    for (int j=-t; j<=t; j++) {
        v += p[i]*j*j;
        i++;
    }
    return v;
}

bool* fibo(int t)
{
    bool f[t];


    return f;
}
