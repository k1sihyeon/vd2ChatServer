#include "widget.h"

#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>
#include <QLabel>

#define BLOCK_SIZE      1024

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    label = new QLabel(this);
    QPushButton *quitBtn = new QPushButton("Quit", this);
    connect(quitBtn, SIGNAL(clicked()), qApp, SLOT(quit()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(clientConnect()));
    if (!server->listen()) {
        QMessageBox::critical(this, tr("Echo Server"), tr("Unable to start the server: %1").arg(server->errorString()));
        close();
        return;
    }

    label->setText(tr("Server is running on port %1.").arg(server->serverPort()));
    setWindowTitle(tr("Chat Server"));
}

void Widget::clientConnect() {
    QTcpSocket *client = server->nextPendingConnection();
    // 서버에 연결 대기중인 client를 가져옴

    clients.push_back(client);

    connect(client, SIGNAL(disconnected()), SLOT(clientDisconnect()));
    connect(client, SIGNAL(readyRead()), SLOT(broadcastChat()));
    label->setText("new connection established!");

    // 입장 알림
    broadcast(client->peerAddress().toString() + " entered chat");
}

void Widget::broadcastChat() {
    // client가 보낸 무언가가 있으면 읽어서 broadcast

    QTcpSocket *client = (QTcpSocket *)sender();
    if (client->bytesAvailable() > BLOCK_SIZE)
        return;

    QByteArray bytearray = client->read(BLOCK_SIZE);

    // 연결된 모든 클라이언트들(배열)에 전송
    for (const auto& client : clients) {
        client->write(bytearray);
    }

    // DB 삽입

    label->setText(QString(bytearray));
}

void Widget::clientDisconnect() {
    QTcpSocket *client = (QTcpSocket *)sender();

    // 퇴장 알림
    broadcast(client->peerAddress().toString() + " left chat");

    client->deleteLater();
    clients.removeOne(client);  // 클라이언트 배열에서 삭제
}

//
void Widget::broadcast(QString msg) {
    QByteArray byte = msg.toUtf8();

    for (const auto& client : clients) {
        client->write(byte);
    }

    label->setText(msg);
}

Widget::~Widget() {}
