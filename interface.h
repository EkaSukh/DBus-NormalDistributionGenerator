#ifndef INTERFACE_H
#define INTERFACE_H

#endif // INTERFACE_H
//#include <QtQml/qqml.h>

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtDBus/QDBusInterface>
#include <QDBusReply>
#include <QRegularExpression>

#define SERVICE_NAME "integer.norm.generator"

class GenInterface: public QObject
{
    Q_OBJECT

 signals:
    void quit();
 public slots:
    void start(const QString &name);

  public:
    GenInterface();
    //QFile qstdin;
    QDBusInterface *iface;
    void setFromCommandLine(QString& inp);


private:
    void getDistribution(const int m, const int d, const int p);
    void getDistribution();
    void printDistribution();

    std::map<int, int> devMap;
    int lastVal;
    int period;
    int defaultMean;
    int defaultDeviation;
    QDBusReply<QDBusVariant> reply;
    QRegularExpression reOne;
    QRegularExpression reTwo;
    QRegularExpression reThree;
    QRegularExpressionMatch match;

    QString lineFromCommandLine;

};
