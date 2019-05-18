#include "myserver.h"
#include "mysocketthread.h"
#include <QMessageBox>
myserver::myserver(QObject * parent):QTcpServer(parent)
{

}

void myserver::incomingConnection(qintptr socketDescriptor){


    mythread * thread = new mythread(this,socketDescriptor);
    qDebug()<<thread;
    connect(thread, SIGNAL(finished()),
    thread, SLOT(deleteLater()));

    connect(thread,SIGNAL(received(QString)),
    this,SLOT(sendMsg(QString)));
    connect(thread,SIGNAL(received_c(QString)),
    this,SLOT(sendMsg_c(QString)));

    thread->start();
}

void myserver::sendMsg(QString s){
    qDebug()<<"ming";
    emit received(s);
}
void myserver::sendMsg_c(QString s){
    emit received_c(s);
    qDebug()<<"myserver";
}
