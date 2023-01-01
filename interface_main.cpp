#include "interface.h"

#include <QCoreApplication>

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QProcess>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    fprintf(stderr, "Interface process is actually started\n");

    if(!QDBusConnection::sessionBus().isConnected()){
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    } else {
        fprintf(stderr, "I connected to session bus from Inerface\n");
    }


    QDBusServiceWatcher serviceWatcher(SERVICE_NAME, QDBusConnection::sessionBus(),
                                       QDBusServiceWatcher::WatchForRegistration);

    GenInterface generatorInterface;

    QObject::connect(&serviceWatcher, &QDBusServiceWatcher::serviceRegistered,
                     &generatorInterface, &GenInterface::start);

   QProcess interf;
   interf.start("./Generator_engine");
/*
   QQmlApplicationEngine engine;
   QQmlContext* rootContext = engine.rootContext();
   rootContext->setContextProperty("classH", &generatorInterface);

   const QUrl qurl("chart.qml");
   QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                    &app, [qurl](QObject *obj, const QUrl & objUrl){
       if(!obj && qurl == objUrl)
               QCoreApplication::exit(-1);
   }, Qt::QueuedConnection);

   engine.load(qurl);


   QQmlComponent component(&engine, QUrl("qrc:chart.qml"));
   auto *genInt = qobject_cast<GenInterface *>(component.create());
    */

   app.exec();
}
