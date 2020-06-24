#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QThread>
#include <QtNetwork>


/**
 * @brief The ClientConnection class
 * Handles I/O from the client
 */
class ClientConnection : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ClientConnection constructs a client connection and connects all relevant signals
     * @param socket the socket ID
     * @param parent the object owning this object
     *
     * @li Create TCP Socket with the socket descriptor
     * @li Connect readReady and disconnected signals
     * @li sets up ds to use TCP Socket
     */
    explicit ClientConnection(qintptr socket, QObject *parent = nullptr);

    /**
     * @brief ~ClientConnection destructor
     * disconnects from the socket if neccessary
     */
    virtual ~ClientConnection();

    /**
     * @brief ClientConnection deleted copy constructor
     */
    ClientConnection(ClientConnection const &) = delete;

    /**
     * @brief operator= deleted assignment operator
     */
    ClientConnection & operator=(ClientConnection const &) = delete;

signals:
    /**
     * @brief receiveMessage emitted on reception of a message
     * @param message the actual message
     */
    void receiveMessage(const QString & message);

    /**
     * @brief disconnected gets emitted when the tcp socket gets disconnected
     * @param client this object
     */
    void disconnected(const ClientConnection* client);
public slots:

    /**
     * @brief onSendMessage gets called from another ClientConnection when they want to forward messages
     * @param message the actual message
     */
    void onSendMessage(const QString & message);

    /**
     * @brief onReadReady gets called from TCP socket when there's data available
     */
    void onReadReady();

    /**
     * @brief onDisconnected gets called from TCP socket when it gets disconnected
     *
     * shedules this object for deletion
     */
    void onDisconnected();

private:
    /**
     * @brief socket socket used for communication
     */
    QTcpSocket * socket;

    /**
     * @brief ds data stream used for serialization of messages
     */
    QDataStream ds;
};

#endif // CLIENTCONNECTION_H
