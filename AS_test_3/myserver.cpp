#include "myserver.h"
#include "mysocketthread.h"
#include <QMessageBox>
myserver::myserver(QObject * parent):QTcpServer(parent)
{

}

void myserver::incomingConnection(qintptr socketDescriptor){

    QMessageBox::about(nullptr,"提示","有新链接");
    mythread * thread = new mythread(this,socketDescriptor);
    qDebug()<<thread;
    connect(thread, SIGNAL(finished()),
    thread, SLOT(deleteLater()));

    connect(thread,SIGNAL(received(QString)),
    this,SLOT(sendMsg(QString)));

    thread->start();
}

void myserver::sendMsg(QString s){
    emit received(s);
}
