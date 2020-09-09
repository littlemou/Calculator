#include "calqulator.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置全局文字大小
    QFont font  = a.font();
    font.setPointSize(15);
    a.setFont(font);
    Calqulator w;
    w.move(550,200);
    w.show();
    w.setWindowTitle("Scientific Calculator");
    return a.exec();
}
