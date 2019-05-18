#include"mysocketthread.h"
#include"as.h"
#include"packetup.h"
#include<QtGui>
#include<QtNetwork>
#include<QTcpServer>
#include<QTcpSocket>
#include<QtSerialPort/QSerialPort>
#include<QString>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlQuery>
#include<QDateTime>

mythread::mythread(QObject * parent,qintptr socketDescriptor)
    :QThread(parent),socketDescriptor(socketDescriptor)
{
    ID_V = new char[3];
    ID_Tgs = new char[3];
    ID_User = new char[3];
    KDCkey = new char[9];
    sessionKey = new char[9];
    UsrKey = new char[9];


    LifeTime = new char[5];
    for(int i =0;i<4;i++){
        LifeTime[i] = '6';
    }
    for(int i = 0;i<8;i++){
        KDCkey[i] = 'k';
        sessionKey[i] = 's';
        UsrKey[i] = 'u';
    }

    LifeTime[4] = '\0';
    KDCkey[8] = '\0';
    sessionKey[8] = '\0';
    UsrKey[8] = '\0';
}

mythread::~mythread(){
    requestInterruption();
    quit();
    wait();
}

void mythread::run(){
    //connet
    connect(this,SIGNAL(send_Messg()),this,SLOT(send_Message()));
    //建立对象
    socket = new QTcpSocket;
    //对象相关功能实现
    qDebug()<<socket;
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit error(socket->error());
        return;
    }
    //读数据
    //socket->waitForReadyRead();
    bool b = connect(socket, SIGNAL(readyRead()),
    this, SLOT(recive_data()),Qt::BlockingQueuedConnection);
    if(b){
        qDebug()<<"yes";
    }
    else{
        qDebug()<<"no";
    }
    exec();

}

void mythread::recive_data(){    
    //接受数据

    QByteArray dataArray = socket->readAll();
    bool Y;
    QByteArray receive;
    for(int i = 0;i<1024;i++){
        receive[i] = dataArray[i];
    }
    qDebug()<<"re  :"<<receive.data();
    QString hash;
    packup->DMsgC_AS(ID_User,LifeTime,ID_V,ID_Tgs,Y,receive,hash);

    //判断rsa解密
    if(Y){
        qDebug()<<"y  :yes";
        emit send_Messg();
    }
    else{
        qDebug()<<"errpr";
        char err[4] = "err";
        socket->write(err,sizeof (err));
    }
    //发送消息打印ui
    QString s("client:\r\n");
    s.append(tr("用户 ID:%1\r\n").arg(ID_User));
    s.append(tr("TGS_ID:%1\r\n").arg(ID_Tgs));
    s.append(tr("服务器 ID:%1\r\n").arg(ID_V));
    s.append(tr("数字签名的hash值:%1\r\n").arg(hash));
    s.append(tr("用户是否合法:%1\r\n").arg(Y));
    emit received(s);
    qDebug()<<s;

}

void mythread::send_Message(){
    qDebug()<<"send_Message";
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();
    qDebug()<<"timeT :"<<timeT;

    //获取时间戳
    char Ts[6];
    for(int i = 0;i<5;i++){
        int t = timeT%10;
        char m[2];
        timeT = timeT/10;
        sprintf(m,"%d",t);
        Ts[i] = m[0];
    }
    Ts[5] = '\0';

    //发送数据
    QByteArray send;
    packup->PMsgAS_C(ID_User,ID_Tgs,Ts,LifeTime,sessionKey,UsrKey,KDCkey,send);
    char sendTOc[1024];
    for(int i =0;i<1024;i++){
        sendTOc[i] = send[i];
    }
    socket->write(sendTOc,sizeof (sendTOc));
    socket->flush();
}























