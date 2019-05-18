#ifndef AS_H
#define AS_H

#include <QMainWindow>
#include"myserver.h"
#include"mysocketthread.h"

namespace Ui {
class AS;
}

class AS : public QMainWindow
{
    Q_OBJECT

public:
    explicit AS(QWidget *parent = nullptr);
    ~AS();

private slots:
    void on_pushButton_clicked();
    void updata(QString s);
private:
    Ui::AS *ui;
    myserver Server;
};

#endif // AS_H
