#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Checkerboard checkerboard;
    checkerboard.show();
    return app.exec();
}
