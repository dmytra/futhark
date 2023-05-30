
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(futhark);

    QApplication app(argc, argv);
    MainWindow window;
    window.loadImage(QStringLiteral(":/futhark.jpg"));
    window.show();
    return app.exec();
}
