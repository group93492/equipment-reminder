#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    //qt translation
    QTranslator qtTranslator;
//    qtTranslator.load("qt_" + QLocale::system().name());
    qtTranslator.load("qt_ru.qm");
    app.installTranslator(&qtTranslator);
    //custom translation
    QTranslator myTranslator;
//    myTranslator.load("translate_" + QLocale::system().name());
    myTranslator.load("translate_ru.ts");
    app.installTranslator(&myTranslator);
    MainWindow w;
    w.show();
    return app.exec();
}
