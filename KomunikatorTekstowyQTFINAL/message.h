#ifndef MESSAGE_H
#define MESSAGE_H


#pragma once
#include <iostream>
#include <QString>
#include <QDateTime>

using namespace std;

class Message : public QObject
{

    Q_OBJECT
private:
    QString m_user;
    QString m_text;
    QDateTime m_timestamp;


public:
    explicit Message(const QString& user, const QString& text, QObject *parent= nullptr);

    QString getTimeStamp() const;
    const QString& getUser() const;
    const QString& getText() const;
};
#endif // MESSAGE_H
