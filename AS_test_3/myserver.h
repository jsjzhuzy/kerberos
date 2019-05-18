#ifndef MYSERVER_H
#define MYSERVER_H
#include"mysocketthread.h"
#include<QTcpServer>
class myserver:public QTcpServer
{
    Q_OBJECT
public:

    explicit myserver(QObject * parent = nullptr);

signals:

    void received(QString s);
private slots:
    void sendMsg(QString s);
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
