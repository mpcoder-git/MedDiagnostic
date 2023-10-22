#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *wincodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(wincodec);
    MainWindow w;
    w.show();

    return a.exec();
}
