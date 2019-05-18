#include "as.h"
#include "ui_as.h"
#include<QMessageBox>
AS::AS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AS)
{
    ui->setupUi(this);
}

AS::~AS()
{
    delete ui;
}

void AS::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    Server.listen(QHostAddress::Any,520);
    connect(&Server,SIGNAL(received(QString)),this,SLOT(updata(QString)));
    connect(&Server,SIGNAL(received_c(QString)),this,SLOT(updata_c(QString)));
}

void AS::updata(QString s){
    ui->textEdit->append(s);
}

void AS::updata_c(QString s){
    qDebug()<<"AS_ui";
    ui->textEdit_2->append(s);
}

