#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QtNetwork>
#include <QList>

#include "clientconnection.h"

/**
 * @brief The could_not_listen exception
 *
 * Will get thrown if the TCPServer can't be bound to the port (already in use)
 */
struct could_not_listen: public std::exception {};


/**
 * @brief The ChatServer class
 *
 * Handles all server related stuff
 * @li listening for incoming connections
 * @li removing dead connections
 */
class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * @brief ChatServer Constructor
     * @param parent the object owning us
     *
     * @li binds the tcp socket to a port
     */
    explicit ChatServer(QObject *parent = nullptr);

    /**
     * @brief ~ChatServer destructor
     *
     * Cleans up all pending connections
     */
    virtual ~ChatServer() override;

    /**
     * @brief ChatServer deleted copy constructor
     */
    ChatServer(ChatServer const &) = delete;

    /**
     * @brief operator= deleted assignment operator
     */
    ChatServer & operator=(ChatServer const &) = delete;

signals:

public slots:
    /**
     * @brief onClientDisconnected gets called when a client gets disconnected
     *
     * removes the client from the clients list
     */
    void onClientDisconnected(const ClientConnection*);

    /**
     * @brief _onDisconnect gets called when we want to disconnect this server
     *
     * shedules this object for deletion
     */
    void _onDisconnect();

protected:
    /**
     * @brief incomingConnection gets called whenever a new connection was opened
     * @param socketDescriptor the TCPSocket descriptor ID
     *
     * constructs a new client connection and links it with the existing ones
     *
     * @li The receiveMessage signal of the new ClientConnection will get linked to the
     *     onSendMessage slot of the existing ClientConnections so incoming messages get distributed
     *
     * @li The onSendMessage slot of the new ClientConnection will get linked to the receiveMessage signal
     *     of the existing ClientConnections, so incoming messages on the other connections will get sent
     *     over this connection
     */
    void incomingConnection(qintptr socketDescriptor) override;

private:
    /**
     * @brief connections stores a list of clients
     */
    QList<const ClientConnection*> connections;
};

#endif // CHATSERVER_H
