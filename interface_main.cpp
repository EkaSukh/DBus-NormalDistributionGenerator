#include "interface.h"

#include <QCoreApplication>

#include <QDBusConnection>
#include <QDBusServiceWatcher>
#include <QProcess>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    fprintf(stderr, "\nInterface process is started\n");

    //construct QString from command line parameters to pass it to GenInterface::start method
    QString input;
    if(argc > 1){
        for(int i = 1; i <=(argc-1); ++i){
            input.append(argv[i]);
            input.append(' ');
        }        
    }

    //check connection to D-Bus
    if(!QDBusConnection::sessionBus().isConnected()){
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    //check if the engine applicatin is connected to D-Bus
    QDBusServiceWatcher serviceWatcher(SERVICE_NAME, QDBusConnection::sessionBus(),
                                       QDBusServiceWatcher::WatchForRegistration);

    //create the interface object and set its string with parameters from command line
    GenInterface generatorInterface;
    generatorInterface.setFromCommandLine(input);

    //set conditions to quit from the application
    QCoreApplication::connect(&generatorInterface, SIGNAL(quit()), &app, SLOT(quit()));


    //as soon as engine is registered its service on D-Bus the interface do its GenInterface::start method
    QObject::connect(&serviceWatcher, &QDBusServiceWatcher::serviceRegistered,
                     &generatorInterface, &GenInterface::start);

    //launch the engine application
    QProcess interf;
    interf.start("./Generator_engine");    

   app.exec();
}
