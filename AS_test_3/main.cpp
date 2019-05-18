#include "as.h"
#include <QApplication>

extern"C"
{
#include<openssl/applink.c>
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AS w;
    w.show();

    return a.exec();
}
