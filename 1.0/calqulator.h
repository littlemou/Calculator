#ifndef CALQULATOR_H
#define CALQULATOR_H

#include <QMainWindow>
#include <calqulator.h>
#include <QPushButton>
namespace Ui {
class Calqulator;
}

class Calqulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calqulator(QWidget *parent = 0);
    ~Calqulator();
public:
    void myslot();

private slots://未完全
    void back_clicked();//AC
    void equal_clicked();//=
    void point_clicked();//.
    void operator_clicked();//
    void specialoperator_clicked();
    void digital_clicked();
    void allclear_clicked();

private:
    Ui::Calqulator *ui;

    void abortoperation();//中止
    void connectslots();//链接信号

    bool waitforoperand;
    void setshortcutkeys();
    QString intopost(QString infix) throw (const char*);//转变为后缀表达式
    double compute(QString s) throw (const char*);//计算后缀表达式结果
    int priority(char a);
    QString error;
    QString log;

};

#endif // CALQULATOR_H
