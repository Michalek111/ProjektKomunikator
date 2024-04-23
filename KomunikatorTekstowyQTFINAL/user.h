#ifndef USER_H
#define USER_H


#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>


using namespace std;

class User : public QObject
{
    Q_OBJECT
private:
    QString m_nick;
    int m_id;
    QString m_password;
    QVector<int> contacts;

public:

    User() ;
    virtual ~User() = default;
    static int next_id;
    explicit User(const QString& nick, int id,const QString& password,QObject *parent = nullptr);
    User(QObject *parent = nullptr);
    const QString& getNick() const;
    const QString& getPassword() const;
    int getId() const;
    bool checkPassword(const QString& password) const;
    void setPassword(const QString& newPassword);
    static int getNextId();
    static void setNextId(int nextId);
    void addContact(int contactId);
    void removeContact(int ConctactId);
    const QVector<int>& getContacts() const;
    void loadContacts();


signals:
    void contactAdded(int contactID);
    void contactRemoved(int contactID);
};
#endif // USER_H
