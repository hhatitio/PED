#include "View/mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <lemon/list_graph.h>
#include <iostream>

using namespace lemon;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if QT_VERSION < 0x050100
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    #endif

    MainWindow w;
    w.show();

    return a.exec();
}
