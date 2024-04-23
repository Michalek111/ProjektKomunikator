#include "userManager.h"
#include "user.h"
#include <QTextStream>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QCoreApplication>
#include <QDir>
#include <QSql>
#include <QSqlDatabase>
using namespace std;

UserManager::~UserManager()
{
    qDeleteAll(m_users);
    m_users.clear();
}

UserManager::UserManager(QObject *parent) : QObject(parent)
{
    QSqlDatabase dateBase = QSqlDatabase::addDatabase("QSQLITE");
     QString databasePath = QCoreApplication::applicationDirPath() + QDir::separator() + "Uzytkownicy1.db";
     dateBase.setDatabaseName(databasePath);
    if (!dateBase.open())
    {
        qDebug() << "Nie można otworzyć bazy danych:" << dateBase.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS Uzytkownicy1 (id INTEGER PRIMARY KEY AUTOINCREMENT, nick TEXT, password TEXT)"))
    {
        qDebug() << "Nie udało się utworzyć tabeli użytkowników:" << query.lastError().text();
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS user_contacts (user_id INTEGER, contact_id INTEGER, PRIMARY KEY (user_id, contact_id), FOREIGN KEY (user_id) REFERENCES Uzytkownicy1(id), FOREIGN KEY (contact_id) REFERENCES Uzytkownicy1(id))"))
    {
        qDebug() << "Nie udało się utworzyć tabeli kontaktów:" << query.lastError().text();
    }
}

void UserManager::loadUser()
{
    if (!dateBase.isOpen())
    {
        qDebug() << "Baza danych nie jest otwarta!";
        return;
    }

    QSqlQuery query;
    if (query.exec("SELECT id, nick, password FROM Uzytkownicy1"))
    {
        while (query.next())
        {
            int id = query.value(0).toInt();
            QString nick = query.value(1).toString();
            QString password = query.value(2).toString();

            User* newUser = new User(nick, id, password, this);
            m_users[nick] = newUser;
        }
    }
    else
    {
        qDebug() << "Błąd wczytywania użytkowników: " << query.lastError().text();
    }
}

QString UserManager::getLoggedInUserNick() const
{
    return loggedInUserNick;
}

int UserManager::getLoggedInUserID(const QString& userNick)
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isOpen()) {
        // Obsługa błędu otwarcia bazy danych
        return -1;
    }

    QSqlQuery query(database);
    query.prepare("SELECT id FROM Uzytkownicy1 WHERE nick = :nick");
    query.bindValue(":nick", userNick);

    if (!query.exec()) {
        // Obsługa błędu wykonania zapytania
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        // Użytkownik o danym nicku nie istnieje
        return -1;
    }
}

bool UserManager::registerUser(const QString& nick, const QString& password)
{
    qDebug() << "registerUser called";
    QSqlQuery queryCheck(dateBase);
    queryCheck.prepare("SELECT * FROM Uzytkownicy1 WHERE nick = :nick");
    queryCheck.bindValue(":nick", nick);

    if (!queryCheck.exec() || queryCheck.next())
    {
        qDebug() << "Rejestracja nieudana - użytkownik o takim nicku już istnieje lub błąd zapytania.";
        return false; // Użytkownik już istnieje lub błąd zapytania
    }

    QSqlQuery queryAdd(dateBase);
    queryAdd.prepare("INSERT INTO Uzytkownicy1 (nick, password) VALUES (:nick, :password)");
    queryAdd.bindValue(":nick", nick);
    queryAdd.bindValue(":password", password);

    if (!queryAdd.exec())
    {
        qDebug() << "Błąd przy dodawaniu użytkownika do bazy danych:" << queryAdd.lastError().text();
        return false;
    }

    qDebug() << "Rejestracja przebiegła pomyślnie.";
    return true;
}


bool UserManager::loginUser(const QString& nick, const QString& password)
{
        QSqlQuery query(dateBase);
        query.prepare("SELECT id, nick, password FROM Uzytkownicy1 WHERE nick = :nick AND password = :password");
        query.bindValue(":nick", nick);
        query.bindValue(":password", password);

        if (!query.exec())
        {
        return false;
        }

        if (query.next()) {
        loggedInUserNick = nick;
        int userId = query.value(0).toInt(); // Pobranie ID użytkownika

        User* loggedInUser = new User(nick, userId, password, this);
        m_users[nick] = loggedInUser;

        loggedInUser->loadContacts(); // Wczytaj kontakty użytkownika

        return true;
        } else {
        return false;
        }
}


void UserManager::addContact(const QString& userNick, int contactId)
{
    User* user = m_users[userNick];
    if (!user) {
        qDebug() << "Błąd: Nie znaleziono użytkownika.";
        return;
    }

    QSqlQuery queryCheck(dateBase);
    queryCheck.prepare("SELECT id FROM Uzytkownicy1 WHERE id = :contactId");
    queryCheck.bindValue(":contactId", contactId);
    if (!queryCheck.exec() || !queryCheck.next())
    {
        qDebug() << "Błąd przy sprawdzaniu istnienia kontaktu lub użytkownik o ID " << contactId << " nie istnieje.";
        return;
    }

    user->addContact(contactId);

    QSqlQuery query;
    query.prepare("INSERT INTO user_contacts (user_id, contact_id) VALUES (:user_id, :contact_id)");
    query.bindValue(":user_id", user->getId());
    query.bindValue(":contact_id", contactId);

    if (!query.exec())
    {
        qDebug() << "Błąd przy dodawaniu kontaktu do bazy danych:" << query.lastError().text();
    }
    else
    {
        qDebug() << "Kontakt został dodany.";
    }
}
void UserManager::removeContact(const QString& userNick, int contactId)
{
    auto userIter = m_users.find(userNick);
    if (userIter != m_users.end())
    {
        User* userPtr = userIter.value();
        if (userPtr)
        {
            // Usuń kontakt z bazy danych
            QSqlQuery query;
            query.prepare("DELETE FROM user_contacts WHERE user_id = :user_id AND contact_id = :contact_id");
            query.bindValue(":user_id", userPtr->getId());
            query.bindValue(":contact_id", contactId);
            if (!query.exec())
            {
                qDebug() << "Błąd przy usuwaniu kontaktu z bazy danych:" << query.lastError().text();
                return;
            }

            // Usuń kontakt z pamięci
            userPtr->removeContact(contactId);

            // Emituj sygnał informujący o usunięciu kontaktu
            //emit contactRemoved(contactId);
        }
    }
    else
    {
        qDebug() << "Użytkownik " << userNick << " nie istnieje.";
    }
}

QString UserManager::getUserNameByID(int id)
{
    for (auto &user : m_users)
    {
        if (user->getId() == id)
        {
            return user->getNick();
        }
    }
    return QString();
}

QVector<int>UserManager::getContactsForUser(const QString& userNick)
{
    User* user = m_users[userNick];
    if(user)
    {
        return user->getContacts();
    }
    return QVector<int>();
}

bool UserManager::isContactExists(const QString& userNick, int contactId) {
    auto userIter = m_users.find(userNick);
    if (userIter == m_users.end()) {
        return false; // Użytkownik nie znaleziony
    }
    User* user = userIter.value();
    const QVector<int>& contacts = user->getContacts();
    return contacts.contains(contactId); // Sprawdź, czy ID kontaktu istnieje w liście kontaktów
}

bool UserManager::changePassword(const QString &oldPassword,const QString &newPassword)
{

    User* loggedInUser = findUserbyNick(getLoggedInUserNick());

    if (!loggedInUser)
    {
        qDebug() << "Brak zalogowanego użytkownika.";
            return false;
    }


    if (loggedInUser->getPassword() != oldPassword)
    {
        qDebug() << "Niepoprawne stare hasło.";
            return false;
    }

    // Zaktualizuj hasło w bazie danych
    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery query(database);
    query.prepare("UPDATE Uzytkownicy1 SET password = :newPassword WHERE nick = :nick");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":nick", loggedInUser->getNick());

    if (!query.exec())
    {
        qDebug() << "Błąd aktualizacji hasła w bazie danych:" << query.lastError().text();
      return false;
    }


    loggedInUser->setPassword(newPassword);

    return true;
}

User* UserManager::findUserbyNick(const QString& nick)
{
    return m_users.value(nick, nullptr);
}
