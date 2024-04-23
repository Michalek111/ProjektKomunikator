#ifndef USERMANAGER_H
#define USERMANAGER_H


#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include "user.h"

using namespace std;


class UserManager : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase dateBase;

    QString loggedInUserNick;
    int loggedInUserID;


public:
    QMap<QString, User*> m_users;
    QVector<int> getContactsForUser(const QString& userNick);
    explicit UserManager(QObject *parent = nullptr);
    virtual ~UserManager();
    bool registerUser(const QString& nick, const QString& password);
    bool loginUser(const QString& nick, const QString& password);
    QString getLoggedInUserNick() const;
    int getLoggedInUserID(const QString& userNick);
    void addContact(const QString& userNick, int contactId);
    void saveUser(const User& user);
    void loadUser();
    QString getUserNameByID(int id);
    bool isContactExists(const QString&, int contactId );
    void removeContact(const QString& userNick, int contactId);
    bool changePassword(const QString &oldPassword,const QString &newPassword);
    User* findUserbyNick(const QString& nick);

signals:
    void registrationSuccess();
    void registrationFailure();

private slots:

};
#endif // USERMANAGER_H
