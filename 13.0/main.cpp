#include "calqulator.h"
#include <QApplication>
#include <QPalette>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置全局文字大小
    QFont font  = a.font();
    font.setPointSize(15);
    a.setFont(font);
    Calqulator w;
    w.move(400,100);
    QPalette pal(w.palette());
    pal.setColor(QPalette::Button, Qt::black);
    w.setAutoFillBackground(true);
    w.setPalette(pal);
    w.show();
    w.setWindowTitle("Scientific Calculator");
    return a.exec();
}
