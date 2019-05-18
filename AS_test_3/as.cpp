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
}

void AS::updata(QString s){
    ui->textEdit->append(s);
}

