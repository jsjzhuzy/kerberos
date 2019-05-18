#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QTextCodec>
#include"descrypt.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_pushButton_Connection_clicked();

    void on_pushButton_Send_clicked();

    void socket_DisConnection();

    void recive_data();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
public:
    char *ID_User;
    char *LifeTime;
    char *ID_V;
    char *ID_Tgs;
    char *SessionKey;
    char *TGT;
    char *UserKey;
};

#endif // MAINWINDOW_H
