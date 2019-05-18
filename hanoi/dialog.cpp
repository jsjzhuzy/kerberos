#include "dialog.h"
#include "ui_dialog.h"
#include "hanoi.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    int num=36;
    output('A','B','C',num);
}

Dialog::~Dialog()
{
    delete ui;
}
