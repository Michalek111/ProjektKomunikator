#include "ChatServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent) {
    tcpServer = new QTcpServer(this);


    connect(tcpServer, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}

void ChatServer::startServer(quint16 port) {
    if (tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port:" << port;
    } else {
        qDebug() << "Server failed to start:" << tcpServer->errorString();
    }
}

void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onDisconnected);

    qDebug() << "New connection from" << clientSocket->peerAddress().toString();
}

void ChatServer::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();

    for (QTcpSocket *client : clients) {
        if (client != clientSocket && client->state() == QTcpSocket::ConnectedState) {
            client->write(data); // Rozsyłanie odbieranej wiadomości do wszystkich połączonych klientów
        }
    }
}

void ChatServer::onDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    qDebug() << "Client disconnected:" << clientSocket->peerAddress().toString();
    clients.removeAll(clientSocket);
    clientSocket->deleteLater();
}
