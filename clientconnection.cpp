#include <QDebug>
#include "clientconnection.h"

ClientConnection::ClientConnection(qintptr socket, QObject *parent) :
    QObject(parent)
{
    this->socket = new QTcpSocket(this);
    this->socket->setSocketDescriptor(socket);

    connect(this->socket,
            &QIODevice::readyRead,
            this,
            &ClientConnection::onReadReady);

    connect(this->socket,
            SIGNAL(disconnected()),
            this,
            SLOT(onDisconnected()));

    ds.setDevice(this->socket);
    ds.setVersion(QDataStream::Qt_5_12);
}

ClientConnection::~ClientConnection()
{
    qDebug() << "Client died!";

    if(this->socket->state() != QAbstractSocket::SocketState::UnconnectedState) {
        this->socket->disconnectFromHost();
        this->socket->waitForDisconnected();
    }

    this->socket->close();

    // the socket will be deleted due to the object hirarchy
}

void ClientConnection::onReadReady()
{
    // gets called on socket as soon as there's data

    QString str;

    // receive something
    ds >> str;

    qDebug() << "ClientConnection::onReadReady running in thread: " << QThread::currentThread()->objectName() << " Message " << str << " @" << &str;

    // emit it on receiveMessage so all other sockets receive it
    emit receiveMessage(str);
}

void ClientConnection::onSendMessage(const QString & message)
{
    qDebug() << "ClientConnection::onSendMessage running in thread: " << QThread::currentThread()->objectName() << " Message: " << message << " @" << &message;

    // send the message over the socket
    ds << message;
}

void ClientConnection::onDisconnected()
{
    emit disconnected(this);

    deleteLater();
}
