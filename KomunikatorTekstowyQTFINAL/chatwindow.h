#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include "ui_chatwindow.h"
#include "NetworkManager.h"
#include "ChatSession.h"
#include "UserManager.h"

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(int userId, const QString &contactNick,QWidget *parent = nullptr,UserManager* userManager=nullptr);
    ~ChatWindow();

private slots:
    void on_sendButton_clicked();
    void sendMessageToServer(const Message &message);
    void displayMessageFromServer(const QByteArray &data);
    void displayMessage(const Message& message);
    void setContactNick(const QString &contactNick);

private:
    Ui::ChatWindow *ui;
    NetworkManager *networkManager;
    ChatSession *chatSession;
    UserManager *userManager;
    QString userNick;
};

#endif // CHATWINDOW_H
