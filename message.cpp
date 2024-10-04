#include "message.h"

Message::Message(QObject *parent) : QObject{parent}
{

}

QByteArray Message::makeBtye() {
    QByteArray bytearray;
    QDataStream stream(&bytearray, QIODevice::WriteOnly);

    // QDataStream으로 직렬화
    stream << this->code << this->msg << this->user.getId() << this->user.getName();

    return bytearray;
}
