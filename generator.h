#ifndef GENERATOR_H
#define GENERATOR_H

#endif // GENERATOR_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>

#include <random>

//this class generates values that are pass to interface allication
class NormGenerator: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "norm.integer.generator")

 public:
    NormGenerator(QObject *obj);
    ~NormGenerator()=default;

public slots:
    //this two fuctions generate values
    QDBusVariant getPoint(const int m, const int d);
    QDBusVariant getPoint();
    //this function is to check communication between engine and interface
    QDBusVariant checkEngine();
    //this function is to quit from the application through D-Bus communication
    Q_NOREPLY void quit();

signals:
    //bounds quit() signal of a parent object
    void toQuit();

private:
    //default parameters for distribution
    const int mean = 10;
    const int deviation = 15;

    std::random_device rd{};
    std::mt19937 gen{rd()};

};
