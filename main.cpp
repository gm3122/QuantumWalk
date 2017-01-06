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
void fibo(int t, bool *f);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // open file in write mode
    QFile file("test.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 1000;

//    qsrand(0);
//    qw2(t, out);
    qw3(t, out);

    qDebug() << "end";

    return a.exec();
}

void qw2(int t, QTextStream &out)
{
    double *p;

    QW2c qw(t);
    double **c = QW2c::getCoin(M_PI/4);

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
    double theta = M_PI/5;

    QW3c *qw;
    double P;
    complex **c= QW3c::getCoin(theta);

    qw = new QW3c(t);
    for (int k=0; k<t; k++) {
        qw->applyCoin(c);
        qw->applyDisplacement();
        P = qw->getReturnAmplitude();
        P -= 1 - sin(theta);
        P -= sqrt(tan(theta)/M_PI)*cos(2*theta*(k+1)+M_PI/4)/sqrt(k+1);
        P *= (k+1)*sqrt(k+1);
        double Q = sqrt(tan(theta)/M_PI)/2.;
        Q *= (tan(theta)-5./tan(theta))/8.;
        Q *= cos(2*theta*(k+1)+M_PI*3/4);
        out << k+1 << " " << P << " " << Q << " " << P-Q << endl;
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

void fibo(int t, bool *f)
{
    f[0] = 0;
    f[1] = 1;
    int k[3] = { 0, 1, 1};
    int q = 2;
    while (1) {
        for (int j=0; j<k[2]; j++) {
            f[q] = f[j];
            q++;
            if (q >= t)
                return;
        }
        k[0] = k[1] + k[2];
        k[1] = k[2];
        k[2] = k[0];
    }
}
