#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void clientConnect();
    void broadcastChat();
    void clientDisconnect();

private:
    QTcpServer *server;
    QLabel *label;
    QVector<QTcpSocket *> clients;

};
#endif // WIDGET_H
