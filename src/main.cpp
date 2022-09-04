#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication applicatoin(argc, argv);
    applicatoin.setWindowIcon(QIcon("./icon.ico"));
    MainWindow mainWindow;
    mainWindow.show();
    return applicatoin.exec();
}
