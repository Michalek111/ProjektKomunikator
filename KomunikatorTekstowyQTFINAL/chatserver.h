#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    void startServer(quint16 port);

signals:
    void newMessage(const QString &message);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket *> clients;
};
