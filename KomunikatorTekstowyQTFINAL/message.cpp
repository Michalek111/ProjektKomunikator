
#include "message.h"

using namespace std;

Message::Message(const QString& user, const QString& text,QObject *parent)
    :QObject(parent),m_user(user), m_text(text)
{
    m_timestamp = QDateTime::currentDateTime();
}

QString Message::getTimeStamp() const
{
    return m_timestamp.toString("yyyy-MM-dd HH:mm:ss");
}

const QString& Message::getUser() const
{
    return m_user;
}

const QString& Message::getText() const
{
    return m_text;
}
