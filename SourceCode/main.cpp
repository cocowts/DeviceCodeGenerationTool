#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("BugNumber");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QTextCodec *codec = QTextCodec::codecForName("Unicode");
     QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    QFont f("微软雅黑", 14);
    a.setFont(f);

    MainWindow w;
    w.show();

    return a.exec();
}
