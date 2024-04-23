#include <algorithm>
#include"user.h"
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>

using namespace std;

int User::next_id = 1000000;

User::User(const QString& nick,int id ,const QString& password, QObject *parent)
    :QObject(parent),m_nick(nick), m_id(id), m_password(password)
{}

const QString& User::getNick() const
{
    return m_nick;
}

int User::getId() const
{
    return m_id;
}
const QString& User::getPassword() const
{
    return m_password;
}


bool User::checkPassword(const QString& password) const
{
    return m_password == password;
}

void User::setPassword(const QString& newPassword)
{
    m_password = newPassword;
}

void User::setNextId(int nextId)
{
    next_id = nextId;
}
int User::getNextId()
{
    return next_id;
}

void User::addContact(int contactId)
{
    if (!contacts.contains(contactId))
    {
        contacts.push_back(contactId);
        emit contactAdded(contactId);
    }
}

void User::removeContact(int contactId)
{
    int index = contacts.indexOf(contactId);
    if(index != -1)
    {
        contacts.removeAt(index);
        emit contactRemoved(contactId);
    }
}

const QVector<int>& User::getContacts() const
{
    return contacts;
}

void User::loadContacts()
{
    QSqlDatabase dateBase = QSqlDatabase::database();
    QSqlQuery query(dateBase);
    query.prepare("SELECT contact_id FROM user_contacts WHERE user_id = :user_id");
    query.bindValue(":user_id", m_id);

    if (query.exec()) {
        while (query.next()) {
            int contactId = query.value(0).toInt();
            contacts.push_back(contactId);
        }
    } else {
        qDebug() << "Błąd wczytywania kontaktów:" << query.lastError().text();
    }
}

