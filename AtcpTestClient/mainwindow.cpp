#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"packetup.h"
#include<QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    QObject::connect(socket,&QTcpSocket::disconnected,this,&MainWindow::socket_DisConnection);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("520");

    bool b = connect(socket, SIGNAL(readyRead()),
    this, SLOT(recive_data()));
    if(b){
        qDebug()<<"yes";
    }
    else{
        qDebug()<<"no";
    }


    //初始化
    LifeTime = new char[5];
    for(int i =0;i<4;i++){
        LifeTime[i] = '6';
    }
    LifeTime[4] = '\0';
    ID_V = new char[3];
    ID_Tgs = new char[3];
    ID_User = new char[3];
    for(int i =0;i<2;i++){
        ID_V[i] = '2';
        ID_Tgs[i] = '1';
        ID_User[i] = '3';
    }
    ID_V[2] = '\0';
    ID_Tgs[2] = '\0';
    ID_User[2] = '\0';

    SessionKey = new char[9];
    UserKey = new char[9];
    TGT = new char[500];
    for(int i = 0;i<8;i++){

        UserKey[i] = 'u';
    }
    UserKey[8] = '\0';
}

MainWindow::~MainWindow()

{
    delete ui;
}

void MainWindow::on_pushButton_Connection_clicked()
{
    if(ui->pushButton_Connection->text() == tr("连接")){
        QString IP;
        int port;
        IP = ui->lineEdit_IP->text();
        port = ui->lineEdit_Port->text().toInt();
        //取消已有连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP,port);
        //等待连接成功
        if(!socket->waitForConnected(30000)){
            qDebug()<<"Connection failed!";
            return;
        }
        qDebug()<<"Connection success!";
        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connection->setText("断开连接");
    }else{
        //断开连接
        socket->disconnectFromHost();
        ui->pushButton_Connection->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()
{    
    //调
    PackUp pack;
    QByteArray send;
    pack.PMsgC_AS(ID_User,LifeTime,ID_V,ID_Tgs,send);

    //兼容中文数据发送
    char sendToas[1024];
    qDebug()<<"send"<<send.data();
    for(int i =0;i<1024;i++){
        sendToas[i] = send[i];
    }
    socket->write(sendToas,sizeof (sendToas));
    socket->flush();
    ui->textEdit_Send->setText(send);
}


void MainWindow::recive_data(){
    qDebug()<<"receive data()";
    //时间戳
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();
    char Ts[6];
    for(int i = 0;i<5;i++){
        int t = timeT%10;
        char m[2];
        timeT = timeT/10;
        sprintf(m,"%d",t);
        Ts[i] = m[0];
    }
    Ts[5] = '\0';
    //读数据
    QByteArray re_B;
    PackUp pack;
    bool Y;
    re_B = socket->readAll();
    qDebug()<<"receive1:"<<re_B.data();
    QByteArray receive;
    for(int i = 0;i<1024;i++){
        receive[i] = re_B[i];
    }
    qDebug()<<"receive2:"<<receive.data();
    qDebug()<<"u_key"<<UserKey;
    pack.DMsgAS_C(ID_Tgs,Ts,LifeTime,SessionKey,UserKey,TGT,Y,receive);
}


void MainWindow::socket_DisConnection()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Connection->setText("连接");
    qDebug()<<"Disconnection!";
}

