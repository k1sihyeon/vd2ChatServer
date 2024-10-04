#include "user.h"

User::User(QObject *parent)
    : QObject{parent}
{}

QString User::getId() {
    return this->id;
}

QString User::getName() {
    return this->name;
}
