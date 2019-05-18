#ifndef MYSOCKETTHREAD_H
#define MYSOCKETTHREAD_H

#include"descrypt.h"
#include<QThread>
#include<QTcpSocket>
#include<QTcpServer>
#include<QtNetwork>
#include"packetup.h"

class QTcpSocket;

class mythread:public QThread{
    Q_OBJECT
public:

    mythread(QObject * parent,qintptr socketDescriptor);
    ~mythread();
    void run();
    void dPackC_As();


signals:

    void received(QString s);
    void error(QTcpSocket::SocketError socketError);
    void search_DB(int a);
    void send_Messg();

public slots:

    void recive_data();
    void send_Message();

private:

    PackUp * packup;
    QTcpSocket * socket;
    qintptr socketDescriptor;
public:

    char *receive_data;
    //send MESSEGE
    char *ID_User;
    char *LifeTime;
    char *ID_V;
    char *ID_Tgs;
    char *sessionKey;
    char *UsrKey;
    char *KDCkey;
};

#endif // MYSOCKETTHREAD_H
