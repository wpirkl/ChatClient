#include <QDebug>

#include "chatmanager.h"

ChatManager::ChatManager(QObject *parent) :
    QObject(parent),
    server(nullptr),
    client(nullptr)
{
    serverThread = new QThread(this);
    serverThread->setObjectName("Server Thread");

    clientThread = new QThread(this);
    clientThread->setObjectName("Client Thread");

    serverThread->start();
    clientThread->start();

    connect(this,
            &ChatManager::connected,
            this,
            &ChatManager::onConnected);

}

ChatManager::~ChatManager()
{
    qDebug() << "Chat Manager died!";
}

void ChatManager::onConnect(const QString & userName, const QString & serverName, bool isServer)
{
    if(connecting) return;      // we're already trying to connect

    connecting = true;

    if(isServer)
    {
        try {
            // create a new chat server
            server = new ChatServer();

            // we want all of this happening in the server thread
            server->moveToThread(serverThread);
        }
        catch(could_not_listen & e)
        {
            qDebug() << "could not listen, so delete server";
            if(server) {
                delete server;
                server = nullptr;
            }
        }
    }

    // creat a new chat client
    client = new ChatClient(userName, serverName);

    // connect the signals to the client
    QObject::connect(this,
                     &ChatManager::sendText,
                     client,
                     &ChatClient::onSendText);

    QObject::connect(client,
                     &ChatClient::textReceived,
                     this,
                     &ChatManager::textReceived);

    connect(client,
            &ChatClient::connected,
            this,
            &ChatManager::connected);

    connect(client,
            &ChatClient::disconnected,
            this,
            &ChatManager::disconnected);

    connect(client,
            SIGNAL(error(const QString &)),
            this,
            SLOT(onClientError(const QString &)));

    // and let it execute within the client thread
    client->moveToThread(clientThread);

    emit client->_connect();
}

void ChatManager::onConnected()
{
    connecting = false;
}

void ChatManager::onDisconnect()
{
    if(server) {
        delete server;
    }

    if(client) {
        delete client;
    }

    server = nullptr;
    client = nullptr;
}

void ChatManager::onSendClicked(const QString & text)
{
    qDebug() << "onSendClicked(" << text << ")";

    // send it to the socket
    emit sendText(text);
}

void ChatManager::onClosing()
{
    if(serverThread) {
        serverThread->exit();
        serverThread->wait(2000);
    }

    if(clientThread) {
        clientThread->exit();
        clientThread->wait(2000);
    }

    serverThread = nullptr;
    clientThread = nullptr;
}

void ChatManager::onClientError(const QString & message)
{
    qDebug() << "ChatManager::onClientError(" << message << ")";
    if(client) {
        emit error(tr("Could not connect to server"));

        client = nullptr;
    }

    connecting = false;
}
