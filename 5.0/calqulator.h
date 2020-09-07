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
    void back_clicked();//back
    void equal_clicked();//=
    void point_clicked();//.
    void operator_clicked();//普通运算符
    void specialoperator_clicked();//特殊运算符
    void digital_clicked();//数字键
    void allclear_clicked();//AC
    void binary_clicked();//二进制
    void hex_clicked();//16进制
    void return_clicked();//ABCDEF返回
    void compare_clicked();//比较大小
    void compare_equal_clicked();//比较大小结果
    void common_divisorbtn_clicked();//公约数
    void common_divisorbtn_equal_clicked();//公约数结果
    void least_common_multiple_clicked();//最小公倍数
    void least_common_multiple_equal_clicked();//最小公倍数结果
    void afunction_2_clicked();//一元二次函数a输入
    void bfunction_2_clicked();//一元二次函数b输入
    void cfunction_2_clicked();//一元二次函数c输入
    void function_2_equal_clidcked();//一元二次函数结果
    void yfunction_a_x_clicked();//幂函数y输入
    void afunction_a_x_clicked();//幂函数a输入
    void function_a_x_equal_clicked();//幂函数结果输出
    void yinversefunction_clicked();//反比例函数y输入
    void kinversefunction_clicked();//反比例函数k输入
    void inversefunction_equal_clicked();//反比例函数结果
    void tofunction_clicked();//转换函数键
    void return_tofunction_clicked();//取消函数，返回特殊符号
    void specialoperator_2_clicked();//特殊函数按键
    void specialoperator_2_equal_clicked();//特殊函数结果
    void tofunction_B_clicked();//和16进制键B重合，改一下
    void hyp_clicked();//转换双曲线函数
    void hyp_return_clicked();//取消双曲
    void hyp_equal_clicked();//双曲结果
    void specialoperator_h_clicked();//双曲按键
    void specialoperator_2h_clicked();//双曲反三角按键
    void specialoperator_2h_equal_clicked();//双曲反三角结果
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
