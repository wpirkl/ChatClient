#include "chatclient.h"

ChatClient::ChatClient(const QString & userName, const QString & serverName, QObject *parent) :
    QObject(parent),
    userName(userName),
    serverName(serverName)
{
    socket = new QTcpSocket(this);

    connect(this->socket,
            &QIODevice::readyRead,
            this,
            &ChatClient::onReadReady);

    connect(this->socket,
            SIGNAL(disconnected()),
            this,
            SLOT(onDisconnected()));

    connect(this->socket,
            SIGNAL(connected()),
            this,
            SIGNAL(connected()));

    connect(this,
            SIGNAL(_connect()),
            this,
            SLOT(_onConnect()));

    connect(this->socket,
            SIGNAL(error(QAbstractSocket::SocketError)),
            this,
            SLOT(onError(QAbstractSocket::SocketError)));

    ds.setDevice(this->socket);
    ds.setVersion(QDataStream::Qt_5_12);
}

ChatClient::~ChatClient()
{
    qDebug() << "Chat Client Died!";

    if(this->socket->state() != QAbstractSocket::SocketState::UnconnectedState) {
        this->socket->disconnectFromHost();
        this->socket->waitForDisconnected();
    }

    this->socket->close();
}

void ChatClient::_onConnect()
{
    socket->connectToHost(serverName, 5000);
}

void ChatClient::onSendText(const QString & text)
{
    qDebug() << "ChatClient::onSendText(" << text << ")";

    ds << QString("%1: %2").arg(userName, text);
}

void ChatClient::onReadReady()
{
    // gets called on socket as soon as there's data

    QString str;

    // receive something
    ds >> str;

    qDebug() << "ChatClient::onReadReady running in thread: " << QThread::currentThread()->objectName() << " Message " << str;

    // emit it on receiveMessage so all other sockets receive it
    emit textReceived(str);
}

void ChatClient::onDisconnected()
{
    qDebug() << "ChatClient::onDisconnected()";

    // we should forward this to the manager
    emit disconnected();
}

void ChatClient::onError(QAbstractSocket::SocketError errorCode)
{
    qDebug() << "ChatClient::onError(" << errorCode << ")";

    emit error(QString(errorCode));

    deleteLater();
}

// qDebug() << "ChatClient::incomingConnection running in thread: " << QThread::currentThread()->objectName();
