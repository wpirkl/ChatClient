#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlComponent>
#include <QQmlContext>
#include <QThread>

#include "uimanager.h"
#include "chatmanager.h"

int main(int argc, char *argv[])
{
    // look nicer on high resolution screens, this has to be done before starting QGuiApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QTranslator translator;
    QLocale locale;
    QLatin1String name("chatclient");
    QLatin1String delim("_");

    qDebug() << locale;

    if(translator.load(locale, name, delim, ":/"))
    {
        qDebug() << "loaded translations";
        app.installTranslator(&translator);
    }

    // set googles material style
    QQuickStyle::setStyle("Material");

    // QML application engine
    QQmlApplicationEngine engine;

    // create a UIManager
    UIManager uiManager(&engine);

    // create ChatManager
    ChatManager chatManager(&engine);

    // set uiManager object in QML
    QQmlContext * rootContext = engine.rootContext();
    rootContext->setContextProperty("uiManager", &uiManager);

    // show main window
    QQmlComponent mainWindowComponent(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    mainWindowComponent.create();

    // connect the 'connect' signal to the chat manager 'onConnect' slot
    QObject::connect(&uiManager,
                     SIGNAL(_connect(const QString &, const QString &, const bool)),
                     &chatManager,
                     SLOT(_onConnect(const QString &, const QString &, const bool)));

    // connect the 'connect' signal to the chat manager 'onConnect' slot
    QObject::connect(&uiManager,
                     SIGNAL(sendClicked(const QString &)),
                     &chatManager,
                     SLOT(onSendClicked(const QString &)));

    // connect the 'disconnect' signal to the chat manager 'onDisconnect' slot
    QObject::connect(&uiManager,
                     SIGNAL(_disconnect()),
                     &chatManager,
                     SIGNAL(_disconnect()));

    // connect the 'closing' signal to the chat manager 'onClosing' slot
    QObject::connect(&uiManager,
                     SIGNAL(closing()),
                     &chatManager,
                     SLOT(onClosing()));

    // connect the 'connected' signal to the chat manager 'connected' slot
    QObject::connect(&chatManager,
                     SIGNAL(connected()),
                     &uiManager,
                     SIGNAL(connected()));

    // connect the 'disconnected' signal to the chat manager 'disconnected' signal
    QObject::connect(&chatManager,
                     SIGNAL(disconnected()),
                     &uiManager,
                     SIGNAL(disconnected()));

    // connect the 'error' signal to the ui manager
    QObject::connect(&chatManager,
                     SIGNAL(error(const QString &)),
                     &uiManager,
                     SIGNAL(error(const QString &)));

    // connect the 'textReceived' signal to the ui manager
    QObject::connect(&chatManager,
                     SIGNAL(textReceived(const QString &)),
                     &uiManager,
                     SIGNAL(textReceived(const QString &)));

    return app.exec();
}
