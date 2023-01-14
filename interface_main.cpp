#include "interface.h"

#include <QCoreApplication>

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QProcess>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    fprintf(stderr, "Interface process is actually started\n");
    fprintf(stderr, "number of parameters is %i\n", argc);
    QString input;
    if(argc > 1){
        for(int i = 1; i <=(argc-1); ++i){
            input.append(argv[i]);
            input.append(' ');
        }
        fprintf(stderr, "Constructed string form input \'%s\'\n", input.toStdString().c_str());
    }


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
    generatorInterface.setFromCommandLine(input);


    QObject::connect(&serviceWatcher, &QDBusServiceWatcher::serviceRegistered,
                     &generatorInterface, &GenInterface::start);

    QCoreApplication::connect(&generatorInterface, SIGNAL(quit()), &app, SLOT(quit()));

   QProcess interf;
   interf.start("./Generator_engine");

   app.exec();
}
