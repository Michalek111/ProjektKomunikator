
#include <algorithm>
#include"chatSession.h"

using namespace std;

ChatSession::ChatSession(QObject *parent) : QObject(parent)
{

}

void ChatSession::addUser(const User& user)
{
    bool userExists = std::any_of(users.begin(), users.end(), [&user](const QSharedPointer<User>& existingUser)
                                  {
        return existingUser->getId() == user.getId();
    });

    if (!userExists) {
        QSharedPointer<User> newUser(new User(user.getNick(), user.getId(), user.getPassword()));
        users.push_back(newUser);
        emit userAdded(*newUser);
    }
}
void ChatSession::removeUser(const User& user) {
    auto it = std::find_if(users.begin(), users.end(),
                           [&user](const QSharedPointer<User>& existingUser) {
                               return existingUser->getId() == user.getId();
                           });

    if (it != users.end()) {
        emit userRemoved(**it); // Użyj **it, aby wysłać referencję do obiektu User
        users.erase(it);
    }
}

void ChatSession::sendMessage(const QString& userNick, const QString& textMessage) {
    auto message = QSharedPointer<Message>::create(userNick, textMessage);
    messages.push_back(message);
    emit newMessage(*message); // Emituj sygnał z nową wiadomością
}

QVector<QSharedPointer<Message>> ChatSession::getMessages() const
{
    return messages;
}


