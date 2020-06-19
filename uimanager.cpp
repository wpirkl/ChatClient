#include <QDebug>
#include <QUrl>
#include <QQmlComponent>

#include "uimanager.h"

UIManager::UIManager(QObject *parent) :
    QObject(parent)
{

}

UIManager::~UIManager()
{
    qDebug() << "UIManager died!";
}

void UIManager::onConnectToServerClicked(const QString & userName, const QString & serverName, const bool isServer)
{
    qDebug() << "onConnectToServer(" << userName << ", " << serverName << ", " << isServer << ")";

    // check if there's a username and a server name
    if(userName.isEmpty()){
        emit error(tr("Specify a username!"));
        return;
    }

    if(serverName.isEmpty()) {
        emit error(tr("Specify a server!"));
        return;
    }

    // clear the info text
    emit error("");

    // emit the connect signal to signal the chat manager 'we want to connect'
    emit connect(userName, serverName, isServer);
}

void UIManager::onSendClicked(const QString & message)
{
    if(message.isEmpty()) return;

    emit sendClicked(message);
}
