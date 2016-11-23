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
void quaternion(double *p, double *a, double *b);
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
    int t = 10000;
    int N = 1;

    double theta = M_PI/4;
    double q_theta[4] = { cos(theta), 0, -sin(theta), 0 };
    double q_k[4] = { 0,0,0,0 };
    double p[4] = { 1,0,0,0 };
    double k;
    double I = 0;
    bool f[t];
    fibo(t,f);
    for (int i=1; i<N; i++) {
        k = i*M_PI/N/2;
        q_k[0] = cos(k);
        q_k[3] = -sin(k);
        p[0] = 1;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        for (int j=0; j<t; j++) {
            theta = f[j] ? M_PI/4 : 0;
            theta = M_PI/4;
            q_theta[0] = cos(theta);
            q_theta[2] = -sin(theta);
            quaternion(p, p, q_theta);
            quaternion(p, p, q_k);
        }
        I += p[3]*p[3]/(1-p[0]*p[0]);
        out << (double) i/N << " " << k << " " << p[3]*p[3]/(1-p[0]*p[0]) << " " << I/N << endl;

//        out << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << endl;
    }
    qDebug() << I/N;
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
    QW3c *qw;
    double theta=M_PI/4;
    double r;
    double S = 0;
    complex **c;
    complex **c1 = QW3c::getCoin(theta);
    complex **c2 = QW3c::getCoin(0);
    bool f[t];
    fibo(t,f);

    qw = new QW3c(t);
    for (int k=0; k<t; k++) {
        c = f[k] ? c1 : c2;
//        c = c1;
        qw->applyCoin(c);
        qw->applyDisplacement();
//        r = qw->getReturnAmplitude() - 1 + sin(theta);
//        r *= std::sqrt(2*M_PI*(k+1));
        S += qw->getReturnAmplitude();
        out << k+1 << " " << qw->getReturnAmplitude() << " " << S << endl;
    }
    qDebug() << qw->getReturnAmplitude();
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

void quaternion(double *p, double *a, double *b)
{
    double _p[4];
    _p[0] = a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3];
    _p[1] = a[0]*b[1] + a[1]*b[0] + a[2]*b[3] - a[3]*b[2];
    _p[2] = a[0]*b[2] + a[2]*b[0] - a[1]*b[3] + a[3]*b[1];
    _p[3] = a[0]*b[3] + a[3]*b[0] + a[1]*b[2] - a[2]*b[1];

    for (int i=0; i<4; i++)
        p[i] = _p[i];
}
