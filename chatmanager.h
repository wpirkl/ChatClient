#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QThread>

#include "chatserver.h"
#include "chatclient.h"

/**
 * @brief The ChatManager class manages the creation / deletion of server and client classes
 */
class ChatManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ChatManager Constructor
     * @param parent object which owns this object
     *
     * Starts server and client thread
     * Connects relevant signals
     */
    explicit ChatManager(QObject *parent = nullptr);

    /**
     * @brief ~ChatManager Destructor
     *
     * Just prints a debug message for now... might be used for more
     */
    virtual ~ChatManager();

    /**
     * @brief ChatManager Copy Constructor is deleted (we don't want copies of this class)
     */
    ChatManager(ChatManager const &) = delete;

    /**
     * @brief operator= assignment operator is deleted (we don't want copies of this class)
     */
    ChatManager & operator=(ChatManager const &) = delete;

signals:
    /**
     * @brief connected signal gets forwarded from the client to the UI (or anybody else who might be interested)
     */
    void connected();

    /**
     * @brief disconnected signal gets forwarded from the client to the UI
     */
    void disconnected();

    /**
     * @brief error gets emitted from the client, whenever there's an error
     * @param message the error message
     */
    void error(const QString & message);

    /**
     * @brief sendText signal forwards the message to the client
     * @param message the actual message
     */
    void sendText(const QString & message);

    /**
     * @brief textReceived gets emitted by the client on reception of a message
     * @param message the actual message
     */
    void textReceived(const QString & message);

    /**
     * @brief _disconnect gets forwarded to the client and the server to tell them to shut down
     */
    void _disconnect();

public slots:
    /**
     * @brief _onConnect start client (and server)
     * @param userName the username for the client
     * @param serverName the servername for the client
     * @param isServer if the server should be started or not
     */
    void _onConnect(const QString & userName, const QString & serverName, bool isServer);

    /**
     * @brief onSendClicked gets called when we should send something
     * @param text actual message
     *
     * we could also connect directly to the client... just to get some debug messages
     */
    void onSendClicked(const QString & text);

    /**
     * @brief onClosing gets called when we should shut down (window closing)
     */
    void onClosing();

    /**
     * @brief onConnected gets called when the client successfully connected
     *
     * just resets the double connection lock
     */
    void onConnected();

    /**
     * @brief onClientError gets called from the client if there's an error
     * @param message error message
     */
    void onClientError(const QString & message);

private:
    /**
     * @brief server the actual chat server
     */
    ChatServer * server;

    /**
     * @brief client the actual chat client
     */
    ChatClient * client;

    /**
     * @brief serverThread server objects context
     */
    QThread * serverThread;

    /**
     * @brief clientThread client objects context
     */
    QThread * clientThread;

    /**
     * @brief connecting we're already trying to connect, so don't do it twice
     */
    bool connecting = false;
};

#endif // CHATMANAGER_H
