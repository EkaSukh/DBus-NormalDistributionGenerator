#ifndef GENERATOR_H
#define GENERATOR_H

#endif // GENERATOR_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>
#include <QMap>

#include <random>

class NormGenerator: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "norm.integer.generator")

 public:
    NormGenerator(QObject *obj);
    ~NormGenerator()=default;

    void testDistribution();
    void printDistribution();

public slots:
   // void setMean(const int m);
   // void setDeviation(const int d);

    QDBusVariant getPoint(const int m, const int d);
    QDBusVariant getPoint();
    QDBusVariant getLastVal();
    QDBusVariant getDistribution();
    QDBusVariant getDistribution(const int m, const int d, const int p);
    QDBusVariant checkEngine();
    Q_NOREPLY void quit();

signals:
    void toQuit();

private:
    const int mean = 0;
    const int deviation = 2;
    const int period = 100;
    std::map<int, int> intDevMap;
    QMap<QString, int> devMap;

    int lastVal;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> distribDefault{(double)mean, (double)deviation};
};
