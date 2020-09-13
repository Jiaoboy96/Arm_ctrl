#include "mainwindow.h"
#include <QApplication>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QObject::connect(w.ui->exit, SIGNAL(clicked()), &a, SLOT(quit())); //关联退出按钮，退出程序

    return a.exec();
}
