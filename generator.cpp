#include "generator.h"

#include <QCoreApplication>
#include <QTimer>
#include <cmath>

NormGenerator::NormGenerator(QObject *obj):QDBusAbstractAdaptor(obj)
{
    fprintf(stderr, "NormGenerator created\n");
    //mean = 10;
    //deviation = 15;
}
/*
void NormGenerator::setMean(const int m)
{
    mean = m;
}
void NormGenerator::setDeviation(const int d)
{
    deviation = d;
}
*/
QDBusVariant NormGenerator::getPoint(const int m, const int d)
{
    fprintf(stderr, "NormGenerator getPoint(const int m, const int d) is called\n");
    std::normal_distribution<> distrib{(double)m, (double)d};
    return QDBusVariant(std::round(distrib(gen)));
}

QDBusVariant NormGenerator::getPoint()
{
    fprintf(stderr, "NormGenerator getPoint() is called\n");
    std::normal_distribution<> distrib{(double)mean, (double)deviation};
    return QDBusVariant(std::round(distrib(gen)));
}

QDBusVariant NormGenerator::checkEngine()
{
    return QDBusVariant("Engine Work");
}
void NormGenerator::quit()
{
    fprintf(stderr, "NormGenerator quit() is called\n");
    QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
}
