#ifndef MESSAGE_H
#define MESSAGE_H

#include "user.h"

#include <QObject>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>

class Message : public QObject
{
    Q_OBJECT

public:
    explicit Message(QObject *parent = nullptr);
    QByteArray makeBtye();


private:
    int code;
    QString msg;
    User user;


signals:

};

#endif // MESSAGE_H
