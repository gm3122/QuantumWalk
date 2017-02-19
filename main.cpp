#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <cmath>

#include "lattice.h"
#include "qw2c.h"
#include "qw3c.h"

const QString folderpath = "../../Plots/";

void simple_qw();
void variance_qw();
void defect_variance_qw();

void qw2(int t, QTextStream &out);
void qw3(int t, QTextStream &out);
double getVariance(double *p, int t);
void fibo(int t, bool *f);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // open file in write mode
//    QFile file("test.dat");
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream out(&file);

    // parameters
//    int t = 11001;

//    qsrand(0);
//    qw2(t, out);
//    qw3(t, out);

//    simple_qw();
    variance_qw();
//    defect_variance_qw();

    qDebug() << "end";

    return a.exec();
}

// Probability density of the quantum walk
void simple_qw()
{
    // open file in write mode
    QFile file(folderpath + "simple_qw.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 100;
    double theta = M_PI/4;
//    double theta = 0.1;

    // variables
    double **c = QW2c::getCoin(theta);
    QW2c qw(t);
    double *p;

    // qw
    for (int i=0; i<t;)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;
    }

    p = qw.getProbabilities();
    int j=0;
    for (int i=-t; i<=t; i++)
    {
        if (i%2 == 0)
            out << (double) i/t << " " << p[j]*t << endl;
        j++;
    }
}

// Variance of the quantum walk
void variance_qw()
{
    // open file in write mode
    QFile file(folderpath + "variance.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 1000;
//    double theta = M_PI/4;
    double theta = M_PI/2 - 1e-2;

    // variables
    double **c = QW2c::getCoin(theta);
    QW2c qw(t);
    double *p;
    double v;

    // qw
    for (int i=0; i<t;)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        p = qw.getProbabilities();
        v = getVariance(p,t);
        out << i << " " << v << endl;
//        out << i << " " << (v - (1-sin(theta))*i*i)  << endl;
    }
}

// Variance of the quantum walk with a defect
void defect_variance_qw()
{
    // open file in write mode
    QFile file(folderpath + "defect_variance.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    double theta = M_PI/4;
//    double eps = 0.5*acos(-1./(1.+2./sin(theta)));
    double eps=M_PI/2;
    int n_1 = 40;
    int n_2 = 1000-40-1;
    int t = n_1 + n_2 + 1;

    // variables
    double **c = QW2c::getCoin(theta);
    double **ce = QW2c::getCoin(theta-eps);
    QW2c qw(t);
    double *p;
    double v;
    int i = 0;

    // qw
    while (i<n_1)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        p = qw.getProbabilities();
        v = getVariance(p,t);
        out << i << " " << v << endl;
    }

    qw.applyCoin(ce);
    qw.applyDisplacement();
    i++;

    p = qw.getProbabilities();
    v = getVariance(p,t);
    out << i << " " << v << endl;

    while (i<t)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        p = qw.getProbabilities();
        v = getVariance(p,t);
        out << i << " " << v << endl;
    }
}

void qw2(int t, QTextStream &out)
{
    double *p;
    double theta = M_PI/4;
    double eps = 0.5*acos(-1./(1.+2./sin(theta)));
    eps = M_PI/2;
    double test = 1-sin(theta)*cos(eps)*cos(eps)-2*(1-sin(theta))*(sin(eps)/cos(theta))*(sin(eps)/cos(theta));
    double **c = QW2c::getCoin(theta);
    double **ce = QW2c::getCoin(theta-eps);

    qDebug() << theta << " " << eps << " " << theta-eps <<" " << test;

    int n_1 = 10;
    int n_2 = 20;
    t = n_1 + n_2 + 1;
    QW2c qw(t);
    for (int i=0; i<n_1; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();

        p = qw.getProbabilities();
        double v = getVariance(p,t);
        out << log(i+1) << " " << log(v) << endl;
    }

    qw.applyCoin(ce);
    qw.applyDisplacement();

    p = qw.getProbabilities();
    double v = getVariance(p,t);
    out << log(n_1+1) << " " << log(v) << endl;

    for (int i=0; i<n_2; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();

        p = qw.getProbabilities();
        double v = getVariance(p,t);
        out << log(n_1+2+i) << " " << log(v) << endl;
    }
}

void qw3(int t, QTextStream &out)
{
    double theta = M_PI/4;
    double eps = 0.5*acos(-1./(1.+2./sin(theta)));

    double P;
    complex **c= QW3c::getCoin(theta);
    complex **ce = QW3c::getCoin(theta-eps);

    QW3c qw(t);

    for (int i=0; i<1000; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();

        P = qw.getReturnAmplitude();
        out << i+1 << " " << P << endl;
    }

    qw.applyCoin(ce);
    qw.applyDisplacement();

    P = qw.getReturnAmplitude();
    out << 1001 << " " << P << endl;

    for (int i=0; i<10000; i++)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();

        P = qw.getReturnAmplitude();
        out << 1002+i << " " << P << endl;
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
