#include "generator.h"

#include <QCoreApplication>
#include <QTimer>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>

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

QDBusVariant NormGenerator::getLastVal()
{
    return QDBusVariant(lastVal);
}

QDBusVariant NormGenerator::getDistribution()
{
    fprintf(stderr, "GetDistribution() is called\n");
    devMap.clear();
    for(int i = 0; i < period; ++i){
        lastVal = std::round(distribDefault(gen));
        //std::cerr << "step " << i << ", generated value " << lastVal << "\n";
        //++intDevMap[lastVal];

        ++devMap[QString((std::to_string(lastVal)).c_str())];
        //std::cerr << "devMap value " << devMap[QString((std::to_string(lastVal)).c_str())] << "\n";
    }

    QMap<QString, QVariant> tmp;
    QMap<QString, int>::iterator i = devMap.begin();
    while(i != devMap.end()){
        tmp.insert(i.key(),QVariant(i.value()));
        ++i;
    }
    std::cerr << "tmp Map:\n";
    QMap<QString, QVariant>::iterator j = tmp.begin();
    while(j != tmp.end()){
        std::cerr << j.key().toStdString() << ' ' << std::string(j.value().toInt(), '*') << "\n";
        ++j;
    }
    QDBusVariant resp(tmp);
    std::cerr << "resp->variant()->toMap().size() " << resp.variant().toMap().size() << "\n";

    return resp;
}

QDBusVariant NormGenerator::getDistribution(const int m, const int d, const int p)
{
    fprintf(stderr, "GetDistribution(const int m, const int d) is called\n");
    devMap.clear();
    std::normal_distribution<> distrib{(double)m, (double) d};
    for(int i = 0; i < p; ++i){

        lastVal = std::round(distrib(gen));
        std::cerr << "step " << i << ", generated value " << lastVal << "\n";
        ++devMap[QString((std::to_string(lastVal)).c_str())];
    }

    QMap<QString, QVariant> tmp;
    QMap<QString, int>::iterator i = devMap.begin();
    while(i != devMap.end()){
        tmp.insert(i.key(),QVariant(i.value()));
        ++i;
    }

    std::cerr << "tmp Map:\n";
    QMap<QString, QVariant>::iterator j = tmp.begin();
    while(j != tmp.end()){
        std::cerr << j.key().toStdString() << ' ' << std::string(j.value().toInt(), '*') << "\n";
        ++j;
    }
    return QDBusVariant(QVariant(tmp));
}

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

void NormGenerator::testDistribution()
{
    /*
    std::cerr << "Input wich distribution to test: 0 or 1(with parameters)\n";
    int v;
    std::cin >> v;
    if(v == 0){
        getDistribution();
        printDistribution();
    }
    else if(v == 1){
        std::cerr << "test distribution with parameters 10 3 500\n";
        getDistribution(10, 3, 500);
        printDistribution();
    }*/

    std::cerr << "Test default distribution \n";
    getDistribution();
    //printDistribution();
    std::cerr << "Test distribution with parameters 10 3 500\n";
    //getDistribution(10, 3, 500);
    //printDistribution();
}

void NormGenerator::printDistribution()
{
    std::cerr << "printDistribution() called\n";
    QMap<QString, int>::iterator i = devMap.begin();
    while(i != devMap.end()){
        std::cerr << i.key().toStdString() << ' ' << std::string(i.value(), '*') << "\n";
        ++i;
    }
    return;
}
