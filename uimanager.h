#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>


class UIManager : public QObject
{
    Q_OBJECT
public:
    explicit UIManager(QObject *parent = nullptr);

    virtual ~UIManager();

    UIManager(UIManager const &) = delete;
    UIManager & operator=(UIManager const &) = delete;

signals:
    void _connect(const QString & username, const QString & serverName, const bool isServer);

    void _disconnect();

    void textReceived(const QString message);

    void connected();

    void disconnected();

    void error(const QString & message);

    void sendClicked(const QString & text);

    void closing();

public slots:
    void onConnectToServerClicked(const QString & userName, const QString & serverName, const bool isServer);
    void onSendClicked(const QString & text);
    void onDisconnectClicked();
};

#endif // UIMANAGER_H
