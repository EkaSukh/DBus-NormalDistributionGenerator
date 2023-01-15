#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>

#include "generator.h"

#define SERVICE_NAME "integer.norm.generator"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QObject obj;
    NormGenerator *generator = new NormGenerator(&obj);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, generator, &NormGenerator::toQuit);

    //register object of session bus
    if(QDBusConnection::sessionBus().registerObject("/norm/generator",&obj)){
        fprintf(stderr, "I registered the object /norm/generator \n");
    }
    //register servise on session bus
    if(!QDBusConnection::sessionBus().registerService(SERVICE_NAME)){
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }else {
        fprintf(stderr, "I registered the service SERVICE_NAME\n");
    }

    app.exec();
    return 0;
}
