#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QThread>
#include <QtNetwork>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(qintptr socket, QObject *parent = nullptr);

    virtual ~ClientConnection();

    // we don't want copy construct
    ClientConnection(ClientConnection const &) = delete;

    // we don't want assignment
    ClientConnection & operator=(ClientConnection const &) = delete;

signals:
    void receiveMessage(const QString & message);

    void disconnected(const ClientConnection* client);
public slots:

    void onSendMessage(const QString & message);

    void onReadReady();

    void onDisconnected();

private:
    QTcpSocket * socket;

    QDataStream ds;
};

#endif // CLIENTCONNECTION_H
