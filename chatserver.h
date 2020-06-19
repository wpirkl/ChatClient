#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QtNetwork>
#include <QList>

#include "clientconnection.h"

struct could_not_listen: public std::exception {};

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

    virtual ~ChatServer() override;

    ChatServer(ChatServer const &) = delete;
    ChatServer & operator=(ChatServer const &) = delete;

signals:

public slots:
    void onClientDisconnected(const ClientConnection*);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    // stores a list of clients
    QList<const ClientConnection*> connections;
};

#endif // CHATSERVER_H
