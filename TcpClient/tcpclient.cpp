#include "tcpclient.h"
#include "descrypt.h"
#include <QMessageBox>
#include <QHostInfo>

TcpClient::TcpClient(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("TCP Client"));

    //contentListWidget = new QListWidget;

    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(tr("发送"));

    userNameLabel = new QLabel(tr("用户名："));
    userNameLineEdit = new QLineEdit;

    serverIPLabel = new QLabel(tr("服务器地址："));
    serverIPLineEdit = new QLineEdit;

    portLabel = new QLabel(tr("端口："));
    portLineEdit = new QLineEdit;

    enterBtn= new QPushButton(tr("连接"));
    enterBtn2=new QPushButton(tr("加密"));
    enterBtn3=new QPushButton(tr("解密"));
    writeLine=new QLineEdit;
    readLine=new QLineEdit;
    mainLayout = new QGridLayout(this);
    //mainLayout->addWidget(contentListWidget,0,0,1,2);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendBtn,1,1);
    mainLayout->addWidget(userNameLabel,2,0);
    mainLayout->addWidget(userNameLineEdit,2,1);
    mainLayout->addWidget(serverIPLabel,3,0);
    mainLayout->addWidget(serverIPLineEdit,3,1);
    mainLayout->addWidget(portLabel,4,0);
    mainLayout->addWidget(portLineEdit,4,1);
    mainLayout->addWidget(enterBtn,5,0,1,2);
    mainLayout->addWidget(enterBtn2,6,0);
    mainLayout->addWidget(enterBtn3,6,1);
    mainLayout->addWidget(readLine,7,0);
    mainLayout->addWidget(writeLine,7,1);

    status = false;

    port = 4444;
    portLineEdit->setText(QString::number(port));

//    serverIP =new QHostAddress(QHostAddress::Any);
    serverIP =new QHostAddress();
    connect(enterBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
    connect(enterBtn2,SIGNAL(clicked()),this,SLOT(encrypt()));
    connect(enterBtn3,SIGNAL(clicked()),this,SLOT(decrypt()));
    sendBtn->setEnabled(false);
}

TcpClient::~TcpClient()
{
    
}
//加密
void TcpClient::encrypt()
{
    QString Qkey = "qwertyui";
    QString data = "hello world!";
    char *key = Qkey.toLatin1().data();
    Des->setKey(key);
    Des->encrypt(data.toLatin1().data());
    QString endata = QString::fromLocal8Bit(Des->endata.c_str());
    readLine->setText(endata);
    //contentListWidget->addItem(endata);
    //ui->keyline->setText(endata);
}
//解密
void TcpClient::decrypt()
{
    QString Qkey = "qwertyui";
    QString data=readLine->text();
    char *key = Qkey.toLatin1().data();
    Des->setKey(key);
    Des->decrypt(data.toLatin1().data());
    QString dedata = QString::fromLocal8Bit(Des->dedata.c_str());
    writeLine->setText(dedata);
    //contentListWidget->addItem(dedata);
}

void TcpClient::slotEnter()
{
    if(!status)
    {
//        QString ip = serverIPLineEdit->text();
        if(!serverIP->setAddress("127.0.0.1"))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }

        if(userNameLineEdit->text()=="")
        {
            QMessageBox::information(this,tr("error"),tr("User name error!"));
            return;
        }

        userName=userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);
        //检测链接信号
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        //检测如果断开
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        //检测如果有新可以读信号
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));

        tcpSocket->connectToHost(*serverIP,port);

        status=true;
    }
    else
    {
        int length=0;
        QString msg=userName+tr(":Leave Chat Room");
        if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg. length())
        {
            return;
        }

        tcpSocket->disconnectFromHost();

        status=false;
    }
}
//链接后
void TcpClient::slotConnected()
{
    sendBtn->setEnabled(true);
    enterBtn->setText(tr("离开"));

    int length=0;
    QString msg=userName+tr(":Enter Chat Room");
    if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        return;
    }
}

void TcpClient::slotSend()
{
    if(sendLineEdit->text()=="")
    {
        return ;
    }

    QString msg=userName+":"+sendLineEdit->text();

    tcpSocket->write(msg.toLatin1(),msg.length());
    sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
    sendBtn->setEnabled(false);
    enterBtn->setText(tr("进入聊天室"));
}

void TcpClient::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(),datagram.size());

        QString msg=datagram.data();
        //contentListWidget->addItem(msg.left(datagram.size()));
    }
}
