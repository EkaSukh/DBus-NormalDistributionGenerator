#include<QCoreApplication>
#include<QDBusVariant>
#include <QDebug>

#include <cstring>
#include <stdio.h>
#include "interface.h"

GenInterface::GenInterface(){
    lastVal = 0;
    period = 1000;
    reOne.setPattern("(\\d+)");
    reTwo.setPattern("(\\d+) (\\d+)");
    reThree.setPattern("(\\d+) (\\d+) (\\d+)");
    defaultMean = 0;
    defaultDeviation = 3;
}

void GenInterface::getDistribution(const int m, const int d, const int p){
    fprintf(stderr, "Get distribution with parameters");
    devMap.clear();
    for(int i = 0; i < p; ++i){
        reply = iface->call("getPoint", m, d);
        lastVal = reply.value().variant().toInt();
        ++devMap[lastVal];
    }
    return;
}

void GenInterface::getDistribution(){
    fprintf(stderr, "Get distribution without parameters");
    devMap.clear();
    for(int i = 0; i < period; ++i){
        reply = iface->call("getPoint");
        lastVal = reply.value().variant().toInt();
        ++devMap[lastVal];
    }
    return;
}

void GenInterface::printDistribution(){
    for(auto i: devMap){
        fprintf(stderr, "%i  %s\n", i.first, std::string(i.second, '*').c_str());
    }
    return;
}


void GenInterface::start(const QString &name)
{
    fprintf(stderr, "GenInterface is called()\n");
    if(name != SERVICE_NAME){
        return;
    }


    qstdin.open(stdin, QIODevice::ReadOnly);
    iface = new QDBusInterface(SERVICE_NAME, "/norm/generator", "norm.integer.generator",
                               QDBusConnection::sessionBus(), this);

    if(!iface->isValid()){
        fprintf(stderr, "%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
        QCoreApplication::instance()->quit();
    }

    connect(iface, SIGNAL(toQuit()), QCoreApplication::instance(), SLOT(quit()));

    QDBusReply<QDBusVariant> startReply = iface->call("checkEngine");
    fprintf(stderr, "Check engine: %s\n", qPrintable(startReply.value().variant().toString()));


    while(true){
        fprintf(stderr, "Start generator interface. Enter your input.\n It should be nothing, or three integers (<mean> <dispersion> <number of points>).\n To quit print \"quit\"\n");


        QString line = QString::fromLocal8Bit(qstdin.readLine()).trimmed();
        if(line.isEmpty()){

            fprintf(stderr, "%s\n", "The string is empty. Calling default parameters");

            getDistribution();
            printDistribution();
            fprintf(stderr, "The last value is %i\n", lastVal);
            continue;

        }else if(line == "quit"){
            fprintf(stderr, "Calling quit\n");
            iface->call("quit");
            emit quit();
            return;

        }else{
            //QRegularExpression re("(\\d+) (\\d+) (\\d+)");
            match = reThree.match(line);
            if(match.hasMatch()){
                const int m = match.captured(1).toInt();
                const int d = match.captured(2).toInt();
                period = match.captured(3).toInt();
                fprintf(stderr, "Mean value is %i, deviation value is %i, number of elements in distribution %i\n", m, d, period);

                getDistribution(m,d,period);
                printDistribution();
                fprintf(stderr, "The last value is %i\n", lastVal);
                continue;

            }else{
                match = reTwo.match(line);
                if(match.hasMatch()){
                    const int m = match.captured(1).toInt();
                    const int d = match.captured(2).toInt();
                    fprintf(stderr, "Mean value is %i, deviation value is %i number of elements in distribution %i\n", m, d, period);

                    getDistribution(m,d,period);
                    printDistribution();
                    fprintf(stderr, "The last value is %i\n", lastVal);
                    continue;

                }else {
                    match = reOne.match(line);
                    if(match.hasMatch()){
                        const int m = match.captured(1).toInt();
                        fprintf(stderr, "Mean value is %i, deviation value is %i number of elements in distribution %i\n", m, defaultDeviation, period);

                        getDistribution(m, defaultDeviation, period);
                        printDistribution();
                        fprintf(stderr, "The last value is %i\n", lastVal);
                        continue;

                    }else{
                        fprintf(stderr, "Could not find any numbers\n");
                                        continue;
                    }
                }
            }
        }
    }
}

/*
 *
*/
