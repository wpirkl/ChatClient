#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QtNetwork>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(const QString & userName, const QString & serverName, QObject *parent = nullptr);

    virtual ~ChatClient();

    ChatClient(ChatClient const &) = delete;
    ChatClient & operator=(ChatClient const &) = delete;

signals:

    void textReceived(const QString & text);

    void _connect();

    void connected();

    void disconnected();

    void error(const QString & message);

public slots:
    void _onConnect();

    void onSendText(const QString & text);

    void onReadReady();

    void onDisconnected();

    void onError(QAbstractSocket::SocketError errorCode);

private:
    QTcpSocket * socket;

    QDataStream ds;
    QString userName;
    QString serverName;
};

#endif // CHATCLIENT_H
