
#ifndef CHATSESSION_H
#define CHATSESSION_H



#pragma once
#include <QObject>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include "message.h"
#include "user.h"

using namespace std;


class ChatSession : public QObject
{
    Q_OBJECT



public:
    explicit ChatSession(QObject *parent = nullptr);
    virtual ~ChatSession() = default;
    void addUser(const User& user);
    void removeUser(const User& user);
    void sendMessage(const QString& userNick, const QString& textMessage);
    QVector<QSharedPointer<Message>> getMessages() const;


signals:

    void userAdded(const User& user);
    void userRemoved(const User& user);
    void messageSend(const Message& message);
    void newMessage(const Message& message);


private:
    QVector<QSharedPointer<User>> users;
    QVector<QSharedPointer<Message>> messages;
};
#endif // CHATSESSION_H

