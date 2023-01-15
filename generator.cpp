#include "generator.h"

#include <QCoreApplication>
#include <QTimer>
#include <cmath>

//constructor has information for debugging process
NormGenerator::NormGenerator(QObject *obj):QDBusAbstractAdaptor(obj)
{
    fprintf(stderr, "NormGenerator created\n");    
}

//produce a value from distribution with users parameters
QDBusVariant NormGenerator::getPoint(const int m, const int d)
{
    fprintf(stderr, "NormGenerator getPoint(const int m, const int d) is called\n");
    std::normal_distribution<> distrib{(double)m, (double)d};
    return QDBusVariant(std::round(distrib(gen)));
}

//produce a value from distribution with default parameters
QDBusVariant NormGenerator::getPoint()
{
    fprintf(stderr, "NormGenerator getPoint() is called\n");
    std::normal_distribution<> distrib{(double)mean, (double)deviation};
    return QDBusVariant(std::round(distrib(gen)));
}

//pass reply to interface application to check comunication
QDBusVariant NormGenerator::checkEngine()
{
    return QDBusVariant("Engine Work");
}

//this function stops the generator application thorough D-Bus comunication
void NormGenerator::quit()
{
    fprintf(stderr, "NormGenerator quit() is called\n");

    QTimer::singleShot(0, QCoreApplication::instance(), &QCoreApplication::quit);
}
