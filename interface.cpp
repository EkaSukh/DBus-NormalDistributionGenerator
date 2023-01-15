#include<QCoreApplication>
#include<QDBusVariant>
#include <QDebug>

#include <cstring>
#include <stdio.h>
#include "interface.h"

//constructor sets most of private parameters
GenInterface::GenInterface(){
    lastVal = 0;
    period = 1000;
    reOne.setPattern("(\\d+)");
    reTwo.setPattern("(\\d+) (\\d+)");
    reThree.setPattern("(\\d+) (\\d+) (\\d+)");
    defaultDeviation = 15;
}

//set private QString to pass to start() function
void GenInterface::setFromCommandLine(QString& inp){
    lineFromCommandLine = inp;
}

//communicate through D-Bus with generator and collects data from the reply
void GenInterface::getDistribution(const int m, const int d, const int p){

    //fprintf(stderr, "Get distribution with parameters\n");
    devMap.clear();
    for(int i = 0; i < p; ++i){
        reply = iface->call("getPoint", m, d);
        lastVal = reply.value().variant().toInt();
        ++devMap[lastVal];
    }
    return;
}

//communicate through D-Bus with generator and collects data from the reply
void GenInterface::getDistribution(){

    //fprintf(stderr, "Get distribution without parameters\n");
    devMap.clear();
    for(int i = 0; i < period; ++i){
        reply = iface->call("getPoint");
        lastVal = reply.value().variant().toInt();
        ++devMap[lastVal];
    }
    return;
}

//prints pseudo graphics about the distribution data
void GenInterface::printDistribution(){
    for(auto i: devMap){
        fprintf(stderr, "%i  %s\n", i.first, std::string(i.second, '*').c_str());
    }
    return;
}

//chech that D-Bus communication between generator and interface aplications works
void GenInterface::start(const QString &name)
{
    //check correctness of SERVICE_NAME    
    if(name != SERVICE_NAME){

        fprintf(stderr, "Service interface is wrong \n");
        //stop both applications
        iface->call("quit");
        emit quit();
        return;
    }

    //initialize the interface variable to D-Bus interaction and check its validity
    iface = new QDBusInterface(SERVICE_NAME, "/norm/generator", "norm.integer.generator",
                               QDBusConnection::sessionBus(), this);
    if(!iface->isValid()){
        fprintf(stderr, "%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
        QCoreApplication::instance()->quit();
    }

    //connects signal in generator application with quit() from generator application
    connect(iface, SIGNAL(toQuit()), QCoreApplication::instance(), SLOT(quit()));

    //check that D-Bus communication between applications works
    QDBusReply<QDBusVariant> startReply = iface->call("checkEngine");
    fprintf(stderr, "Check engine: %s\n", qPrintable(startReply.value().variant().toString()));


    //now the main part of the function do the D-Bus communication

        fprintf(stderr, "Enter your input. It should be nothing, or three integers (<mean> <dispersion> <number of points>).\n");

        QString line = lineFromCommandLine;
        if(line.isEmpty()){

            fprintf(stderr, "Input is empty. Use default parameters.\n Mean value is 10, deviation value is 15, number of elements in distribution 1000\n");

            getDistribution();
            printDistribution();
            fprintf(stderr, "The last value is %i\n", lastVal);

            //quit from both applications
            iface->call("quit");
            emit quit();
            return;

        }else{

            match = reThree.match(line);
            if(match.hasMatch()){
                fprintf(stderr, "Input is: %s\n", lineFromCommandLine.toStdString().c_str());
                const int m = match.captured(1).toInt();
                const int d = match.captured(2).toInt();
                period = match.captured(3).toInt();
                fprintf(stderr, "Mean value is %i, deviation value is %i, number of elements in distribution %i\n", m, d, period);

                getDistribution(m,d,period);
                printDistribution();
                fprintf(stderr, "The last value is %i\n", lastVal);

                //quit from both applications
                iface->call("quit");
                emit quit();
                return;

            }else{

                match = reTwo.match(line);
                if(match.hasMatch()){
                    fprintf(stderr, "Input is: %s\n", lineFromCommandLine.toStdString().c_str());
                    const int m = match.captured(1).toInt();
                    const int d = match.captured(2).toInt();
                    fprintf(stderr, "Mean value is %i, deviation value is %i number of elements in distribution %i\n", m, d, period);

                    getDistribution(m,d,period);
                    printDistribution();
                    fprintf(stderr, "The last value is %i\n", lastVal);

                    //quit from both applications
                    iface->call("quit");
                    emit quit();
                    return;

                }else {

                    match = reOne.match(line);
                    if(match.hasMatch()){
                        fprintf(stderr, "Input is: %s\n", lineFromCommandLine.toStdString().c_str());
                        const int m = match.captured(1).toInt();
                        fprintf(stderr, "Mean value is %i, deviation value is %i number of elements in distribution %i\n", m, defaultDeviation, period);

                        getDistribution(m, defaultDeviation, period);
                        printDistribution();
                        fprintf(stderr, "The last value is %i\n", lastVal);

                        //quit from both applications
                        iface->call("quit");
                        emit quit();
                        return;

                    }else{
                        fprintf(stderr, "Input is: %s\n", lineFromCommandLine.toStdString().c_str());
                        fprintf(stderr, "Could not find any numbers\n");

                        //quit from both applications
                        iface->call("quit");
                        emit quit();
                        return;
                    }
                }
            }
        }

}

