#include "dio.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dio w;
    w.show();

    return a.exec();
}
