#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QThread>

#include "chatserver.h"
#include "chatclient.h"


class ChatManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatManager(QObject *parent = nullptr);

    virtual ~ChatManager();

    ChatManager(ChatManager const &) = delete;
    ChatManager & operator=(ChatManager const &) = delete;

signals:
    void connected();

    void disconnected();

    void error(const QString & message);

    void sendText(const QString & message);

    void textReceived(const QString & message);

public slots:
    void onConnect(const QString & userName, const QString & serverName, bool isServer);

    void onDisconnect();

    void onSendClicked(const QString & text);

    void onClosing();

    void onConnected();

    void onClientError(const QString & message);

private:
    ChatServer * server;

    ChatClient * client;

    QThread * serverThread;

    QThread * clientThread;

    bool connecting = false;
};

#endif // CHATMANAGER_H
