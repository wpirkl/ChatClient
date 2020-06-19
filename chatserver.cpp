#include <QDebug>
#include "chatserver.h"

ChatServer::ChatServer(QObject *parent) :
    QTcpServer(parent)
{
    // listen to port 000
    if(!listen(QHostAddress::Any, 5000)) {
        qDebug() << "Could not listen!";
        throw could_not_listen();
    }
}

ChatServer::~ChatServer()
{
    qDebug() << "Chat Server Destroyed";

    // delete the objects we created
    for(const ClientConnection * connection : connections)
    {
        delete connection;
    }

    connections.clear();
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "ChatServer::incomingConnection running in thread: " << QThread::currentThread()->objectName();

    ClientConnection * connection = new ClientConnection(socketDescriptor, this);

    for(const ClientConnection * other : connections)
    {
        // so if others receive a message, send it out on this one
        QObject::connect(other,
                         SIGNAL(receiveMessage(const QString &)),
                         connection,
                         SLOT(onSendMessage(const QString &)));

        // and if this one receives a message, send it out on the others
        QObject::connect(connection,
                         SIGNAL(receiveMessage(const QString &)),
                         other,
                         SLOT(onSendMessage(const QString &)));
    }

    // the client disconnect signal
    QObject::connect(connection,
                     SIGNAL(disconnected(const ClientConnection*)),
                     this,
                     SLOT(onClientDisconnected(const ClientConnection*)));

    // and now append this socket to the list of connected clients
    connections.append(connection);
}

void ChatServer::onClientDisconnected(const ClientConnection* client)
{
    connections.removeAll(client);
}
