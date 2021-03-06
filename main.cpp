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
void variance_plot();
void defect_variance_qw();
void defect_qw3();

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
//    variance_qw();
//    variance_plot();
//    defect_variance_qw();
    defect_qw3();

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
//    QFile file(folderpath + "variance2.dat");
    QFile file(folderpath + "test_var.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 10000;
//    double theta = M_PI/4;
//    double theta = M_PI/2 - 1e-2;
    double theta = 1.5;

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

        double var = (1-sin(theta))*i*i;
        var += (1+sin(theta)*sin(theta))/(4*sin(theta));
        var -= sqrt(tan(theta)/M_PI/i)/2/sin(theta)/sin(theta)*cos(2*theta*i+M_PI/4);
//        var -= sqrt(tan(theta)/M_PI/i)/2/sin(theta)/sin(theta)*cos(2*theta*i+3*M_PI/4)
//                *(tan(theta)-13/tan(theta))/(16*i);
        out << i << " " << (var - v)*sqrt(i) << endl;

//        out << i << " " << v << endl;
//        out << i << " " << QString::number(v, 'g', 17)
//                 << " " << QString::number((1-sin(theta))*i*i, 'g', 17)
//                 << " " << v - (1-sin(theta))*i*i
//            << endl;
//        out << i << " " << (v - (1-sin(theta))*i*i)  << endl;
    }
}

// Variance of the quantum walk
void variance_plot()
{
    // open file in write mode
    QFile file(folderpath + "variance_plot2.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    int t = 30;
    int N = 2000;
    double theta;

    // variables
    QW2c *qw;
    double **c;
    double *p;
    double v;

    for (int j=0; j<N; j++)
    {
        theta = j*M_PI/N/2;
        c = QW2c::getCoin(theta);
        qw = new QW2c(t);
        // qw
        for (int i=0; i<t; i++)
        {
            qw->applyCoin(c);
            qw->applyDisplacement();
        }

        p = qw->getProbabilities();
        v = getVariance(p,t);
        v = 1-(1-sin(theta))*t*t/v;

        delete c;
        delete qw;

        qDebug() << theta << v;
        out << theta << " " << v << endl;

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
    double eps = M_PI/2;
    int n_1 = 40;
    int t = 1000;

    // variables
    double **c = QW2c::getCoin(theta);
    double **ce = QW2c::getCoin(theta-eps);
    QW2c qw(t);
    double *p;
    double v;
    int i = 0;

    // analytical asymptote
    double a[t+1];
    double a_1 = 1-std::abs(sin(theta));
    double a_2 = 1-std::abs(sin(theta))*cos(eps)*cos(eps)
                  -2*a_1*sin(eps)*sin(eps)/cos(theta)/cos(theta);
    for (int j=0; j<n_1; j++)
        a[j] = a_1*j*j;
    for (int j=n_1; j<=t; j++)
        a[j] = a_1*(n_1*n_1+(j-n_1)*(j-n_1))+a_2*2*(j-n_1)*n_1;

    // qw
    while (i<n_1)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        p = qw.getProbabilities();
        v = getVariance(p,t);
        out << i << " " << v << " " << a[i] << " " << std::abs(1 - a[i]/v) << endl;
    }

    qw.applyCoin(ce);
    qw.applyDisplacement();
    i++;

    p = qw.getProbabilities();
    v = getVariance(p,t);
    out << i << " " << v << " " << a[i] << " " << std::abs(1 - a[i]/v) << endl;

    while (i<t)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        p = qw.getProbabilities();
        v = getVariance(p,t);
        out << i << " " << v << " " << a[i] << " " << std::abs(1 - a[i]/v) << endl;
    }
}

void defect_qw3()
{
    // open file in write mode
    QFile file(folderpath + "defect_qw3.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    // parameters
    double theta = M_PI/4;
    double eps = M_PI/2;
    int n_1 = 40;
    int t = 100;

    // variables
    int i = 0;
    double P;
    complex **c= QW3c::getCoin(theta);
    complex **ce = QW3c::getCoin(theta-eps);
    QW3c qw(t);

    double a_theta = 1 - std::abs(sin(theta));
    double a_eps = 1 - std::abs(sin(theta))*cos(eps)*cos(eps) - 2*a_theta*sin(eps)*sin(eps)/cos(theta)/cos(theta);

    // qw
    while (i<n_1)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        P = qw.getReturnAmplitude();
        out << i << " " << P << " " << a_theta << endl;
    }

    qw.applyCoin(ce);
    qw.applyDisplacement();
    i++;

    P = qw.getReturnAmplitude();
    out << i << " " << P << " " << a_eps << endl;

    while (i<t)
    {
        qw.applyCoin(c);
        qw.applyDisplacement();
        i++;

        P = qw.getReturnAmplitude();
        out << i << " " << P << " " << a_eps << endl;
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
