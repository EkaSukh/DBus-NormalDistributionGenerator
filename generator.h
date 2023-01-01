#ifndef GENERATOR_H
#define GENERATOR_H

#endif // GENERATOR_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>

#include <random>

class NormGenerator: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "norm.integer.generator")

 public:
    NormGenerator(QObject *obj);
    ~NormGenerator()=default;

public slots:
   // void setMean(const int m);
   // void setDeviation(const int d);

    QDBusVariant getPoint(const int m, const int d);
    QDBusVariant getPoint();
    QDBusVariant checkEngine();
    Q_NOREPLY void quit();

signals:
    void toQuit();

private:
    const int mean = 10;
    const int deviation = 15;

    std::random_device rd{};
    std::mt19937 gen{rd()};

};
