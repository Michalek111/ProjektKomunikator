#include "NetworkManager.h"
#include <QDebug>
#include <QTcpSocket>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &NetworkManager::onError);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
}

void NetworkManager::connectToServer()
{
    QString serverIp = "192.168.1.105";
    quint16 serverPort = 1234;
    tcpSocket->connectToHost(serverIp, serverPort);
}

void NetworkManager::sendData(const QByteArray &data)
{   if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
    tcpSocket->write(data);
    }
}

void NetworkManager::onConnected() {
    qDebug() << "Connected to server.";
    emit connected();
}

void NetworkManager::onDisconnected() {
    qDebug() << "Disconnected from server.";
    emit disconnected();
}

void NetworkManager::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    qDebug() << "Network error: " << tcpSocket->errorString();
    emit errorOccurred(tcpSocket->errorString());
}

void NetworkManager::onReadyRead() {
    QByteArray data = tcpSocket->readAll();
    emit receivedData(data);
}


