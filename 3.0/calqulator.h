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
    void binary_clicked();
    void hex_clicked();
    void return_clicked();
    void compare_clicked();
    void compare_equal_clicked();
    void common_divisorbtn_clicked();
    void common_divisorbtn_equal_clicked();
    void least_common_multiple_clicked();
    void least_common_multiple_equal_clicked();
    void afunction_2_clicked();
    void bfunction_2_clicked();
    void cfunction_2_clicked();
    void function_2_equal_clidcked();
    void yfunction_a_x_clicked();
    void afunction_a_x_clicked();
    void function_a_x_equal_clicked();
    void yinversefunction_clicked();
    void kinversefunction_clicked();
    void inversefunction_equal_clicked();
    void tofunction_clicked();
    void return_tofunction_clicked();
    void specialoperator_2_clicked();
    void specialoperator_2_equal_clicked();
    void tofunction_B_clicked();
    void hyp_clicked();
    void hyp_return_clicked();
    void hyp_equal_clicked();
    void specialoperator_h_clicked();
    private:
    Ui::Calqulator *ui;
    void abortoperation();//中止
    void connectslots();//链接信号
    bool ok_change=true;
    bool waitforoperand;
    QString bin_change(int i,QString value);
    QString hex_change(int i,QString value);
    QString intopost(QString infix);//转变为后缀表达式
    double compute(QString s);//计算后缀表达式结果
    int priority(char a);
    QString error;
    QString flag="0";
};

#endif // CALQULATOR_H
