#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QtNetwork>

/**
 * @brief The ChatClient class
 * @author Werner Pirkl
 *
 * This class handles the client side of the chat service
 */
class ChatClient : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ChatClient
     * @param userName The user used to transmit messages (will just be in front of the message)
     * @param serverName The server to connect to
     * @param parent The object which owns the newly created ChatClient object
     *
     * Creates the socket and connects the signals of the socket to signals or slots whithin this class
     * DataStream will aso be assigned to the socket.
     */
    explicit ChatClient(const QString & userName, const QString & serverName, QObject *parent = nullptr);

    /**
     * @brief ~ChatClient destructor
     *
     * Cleans up this object. If the connection is not closed, it will do so
     */
    virtual ~ChatClient();

    /**
     * @brief ChatClient copy constructor is deleted, as it is not intended to copy this connection
     */
    ChatClient(ChatClient const &) = delete;

    /**
     * @brief operator= assignment operator is also deleted, as it makes no sense do create clones of this object
     */
    ChatClient & operator=(ChatClient const &) = delete;

signals:

    /**
     * @brief textReceived is emitted on successful reception of a message
     * @param text the message itself
     */
    void textReceived(const QString & text);

    /**
     * @brief _connect signal will be emitted by the chat manager once this client should try to connect
     */
    void _connect();

    /**
     * @brief connected signal will be emitted on successful connection by the socket
     */
    void connected();

    /**
     * @brief disconnected the socket will emit this signal when it gets disconnected (connection loss)
     */
    void disconnected();

    /**
     * @brief error This signal gets emitted when the socket signals an error.
     * @param message The error message
     */
    void error(const QString & message);

public slots:
    /**
     * @brief _onConnect gets called from the _connect signal and starts connecting to the specified host
     */
    void _onConnect();

    /**
     * @brief _onDisconnect gets called from outside this class to request a disconnection
     *
     * It shedules this class for deletion
     */
    void _onDisconnect();

    /**
     * @brief onSendText gets called from an outside signal and sends the text over the connection. The username is prefixed
     * @param text message to send
     */
    void onSendText(const QString & text);

    /**
     * @brief onReadReady called by socket whenever there's something on the line to read
     */
    void onReadReady();

    /**
     * @brief onDisconnected called by socket when it gets disconnected.
     *
     * Forwards the information to disconnected signal and shedules this object for deletion
     */
    void onDisconnected();

    /**
     * @brief onError called by socket on error (maybe server is not accepting connections)
     * @param errorCode not used.
     *
     * Forwards the information to error signal and shedules this object for deletion
     */
    void onError(QAbstractSocket::SocketError errorCode);

private:
    /**
     * @brief socket Socket object used for communication
     */
    QTcpSocket * socket;

    /**
     * @brief ds Used to serialize and deserialize incoming messages
     */
    QDataStream ds;

    /**
     * @brief userName Username to be used to prefix our messages
     */
    QString userName;

    /**
     * @brief serverName Hostname of the server to connect to
     */
    QString serverName;
};

#endif // CHATCLIENT_H
