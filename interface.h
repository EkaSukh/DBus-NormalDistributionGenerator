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

//this class is an interface that communicate with generator engine application
class GenInterface: public QObject
{
    Q_OBJECT

 signals:
    //signal to quit from interface application
    void quit();

 public slots:    
    //send request to engine application and process the reply
    //as a parameter pass SERVICE_NAME that matches service.name for generator application
    void start(const QString &name);

  public:
    GenInterface();
    ~GenInterface()=default;

    //interface to communicate through D-Bus with engine application
    QDBusInterface *iface;

    //set lineFromCommandLine private variable
    void setFromCommandLine(QString &inp);


private:
    //send request through D-Bus to engine application and collects the reply
    void getDistribution(const int m, const int d, const int p);
    void getDistribution();

    //pring a pseudo graphic information about requested distribution
    void printDistribution();

    //variable to store distributin values
    std::map<int, int> devMap;
    //last value generated in distribution
    int lastVal;

    //default distribution parameters    
    int defaultDeviation;
    int period;    

    //variable to store reply got from D-Bus communication
    QDBusReply<QDBusVariant> reply;

    //variables to process line of users parameters
    QRegularExpression reOne;
    QRegularExpression reTwo;
    QRegularExpression reThree;
    QRegularExpressionMatch match;

    //line to pass to start() function
    QString lineFromCommandLine;

};
