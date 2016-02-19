#include "View/mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <lemon/list_graph.h>
#include <iostream>

using namespace lemon;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    MainWindow w;
    w.show();

    return a.exec();
}
