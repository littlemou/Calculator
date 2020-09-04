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
    void compare_equal_clicked();
    void point_clicked();//.
    void operator_clicked();//
    void specialoperator_clicked();
    void digital_clicked();
    void allclear_clicked();
    void binary_clicked();
    void hex_clicked();
    void return_clicked();
    void compare_clicked();
private:
    Ui::Calqulator *ui;

    void abortoperation();//中止
    void connectslots();//链接信号
    bool ok_change=true;
    bool waitforoperand;
    QString intopost(QString infix) throw (const char*);//转变为后缀表达式
    double compute(QString s) throw (const char*);//计算后缀表达式结果
    QString bin_change(int i,QString value);
    QString hex_change(int i,QString value);
    int priority(char a);
    QString error;
};

#endif // CALQULATOR_H
