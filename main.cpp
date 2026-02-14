#include "smartmarket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartMarket w;  // fenêtre de main
    w.show();
    return a.exec();
}
