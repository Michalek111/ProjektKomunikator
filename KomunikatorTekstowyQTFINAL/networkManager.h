#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>


class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    //void connectToServer(const QString &host, quint16 port);
    void connectToServer();
    void sendData(const QByteArray &data);

signals:
    void receivedData(const QByteArray &data);
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorString);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead();


private:
    QTcpSocket *tcpSocket;
};


#endif // NETWORKMANAGER_H
