#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>
#include"descrypt.h"
class TcpClient : public QDialog
{
    Q_OBJECT
    
public:
    TcpClient(QWidget *parent = 0,Qt::WindowFlags f=0);
    ~TcpClient();
private:
   // QListWidget *contentListWidget;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;
    QLabel *serverIPLabel;
    QLineEdit *serverIPLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QPushButton *enterBtn;
    QPushButton *enterBtn2;
    QPushButton *enterBtn3;
    QLineEdit *writeLine;
    QLineEdit *readLine;
    QGridLayout *mainLayout;
    bool status;
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpSocket;
    DesCrypt *Des=new DesCrypt();
public slots:
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();
    void encrypt();
    void decrypt();
};

#endif // TCPCLIENT_H
