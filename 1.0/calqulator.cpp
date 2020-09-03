#include "calqulator.h"
#include "ui_calqulator.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <stack>
#include <cmath>
#include <map>
#include <QApplication>
#include <queue>
#include <QString>
#include <iostream>
Calqulator::Calqulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calqulator)
{
    ui->setupUi(this);
    this->setFixedSize(800,500);
    this->move(500,1000);
    ui->lineEdit->setText("0");
    waitforoperand=true;//等待计算
    connectslots();
}
Calqulator::~Calqulator()
{
    delete ui;
}
void Calqulator::abortoperation()//中止函数
{
    ui->lineEdit->setText("0");
    waitforoperand=true;
    QMessageBox::warning(this,"运算错误","wrong!");

}
void Calqulator::connectslots()//将按钮与信号连接
{
    QPushButton *digitalbtn[10]={ui->digitalbtn0,ui->digitalbtn1,ui->digitalbtn2
                                ,ui->digitalbtn3,ui->digitalbtn4,ui->digitalbtn5
                                ,ui->digitalbtn6,ui->digitalbtn7,ui->digitalbtn8
                                ,ui->digitalbtn9};
    for(auto btn:digitalbtn)//数字依次与信号连接
        connect(btn,&QPushButton::clicked,this,&Calqulator::digital_clicked);

    QPushButton *operatorbtn[8]={ui->addbtn,ui->subbtn,ui->mulbtn,ui->divbtn
                                ,ui->leftbracketbtn,ui->rightbracketbtn
                                ,ui->percentbtn,ui->pawbtn};
    for(auto btn:operatorbtn)//+-*/()%^   //60
        connect(btn,&QPushButton::clicked,this,&Calqulator::operator_clicked);

    QPushButton *specialoperatorbtn[5]={ui->sinbtn,ui->cosbtn,ui->tanbtn,
                                       ui->lnbtn,ui->lgbtn};
    for(auto btn:specialoperatorbtn)//ln,lg,sin,cos,tan
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    connect(ui->backbtn,&QPushButton::clicked,this,&Calqulator::back_clicked);
    connect(ui->allclearbtn,&QPushButton::clicked,this,&Calqulator::allclear_clicked);
    connect(ui->pointbtn,&QPushButton::clicked,this,&Calqulator::point_clicked);
    connect(ui->binbtn,&QPushButton::clicked,this,&Calqulator::binary_clicked);
}
void Calqulator::digital_clicked()//字符串输入显示 60
{
    QPushButton *digitbtn=static_cast<QPushButton*>(sender());//判断发出对象????
    QString value=digitbtn->text();
    if(ui->lineEdit->text()=="0"&&value=="0")//初始为0则不动
    {
        return;
    }
    if(waitforoperand==true)//如果输入框为空
    {
        ui->lineEdit->setText(value);
        waitforoperand=false;
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);//所有的一起输出
    }
}
void Calqulator::operator_clicked()//60
{
    QPushButton *operatorbtn=qobject_cast<QPushButton*>(sender());//?????
    QString value=operatorbtn->text();
    if(waitforoperand==true&&value=="(")
    {
        ui->lineEdit->setText(value);
        waitforoperand=false;
    }
    else
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}
void Calqulator::specialoperator_clicked()
{
    QPushButton *specialoperatorbtn=static_cast<QPushButton*>(sender());
    QString value=specialoperatorbtn->text();
    if(waitforoperand==true)
    {
        ui->lineEdit->setText(value);
        waitforoperand=false;
    }
    else
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}
void Calqulator::equal_clicked()
{
    double result=0;
    int number;
    QString value=ui->lineEdit->text();//value之后会是二进制转换之后的字符串
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='b')
        {
            int bin=i+1;
            for(number=0;value[bin]=='0'||value[bin]=='1';bin++)
            {
                number++;
            }
            QString str=value.mid(i+1,number);//二进制字符串
            qDebug()<<str;
            int binstr_dec=str.toInt(&ok_change,10);
            value=value.replace(i-1,i+1+number,binstr_dec);//用str替换i-1到i+1+number的字符
        }
    }
    result=compute(intopost(value));//自写函数
    ui->lineEdit->setText((ui->lineEdit->text()+'='+QString::number(result)));
    log=ui->lineEdit->text();//string
    waitforoperand=true;//重置
}
void Calqulator::back_clicked()//回车
{
    QString value=ui->lineEdit->text();
    if(value!='0'||waitforoperand==false)
    {
        ui->lineEdit->setText(value.left(value.count()-1));//左数减一位输出
    }
    else
    {
        ui->lineEdit->setText("0");
        waitforoperand=true;
    }
}
void Calqulator::allclear_clicked()//清0
{
    ui->lineEdit->setText("0");
    waitforoperand=true;
}
void Calqulator::point_clicked()
{
    if(waitforoperand==false&&ui->lineEdit->text().data()[ui->lineEdit->text().size()-1].isDigit()==true)
    {
        ui->lineEdit->setText(ui->lineEdit->text()+".");
    }
}
void Calqulator::binary_clicked()
{
    QPushButton *digitbtn=static_cast<QPushButton*>(sender());//判断发出对象
    QString value=digitbtn->text();
    if(ui->lineEdit->text()=="0"&&value=="0")//初始为0则不动
    {
        return;
    }
    if(waitforoperand==true)//如果输入框为空
    {
        ui->lineEdit->setText(value);
        waitforoperand=false;
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);//所有的一起输出
    }
}
int Calqulator::priority(char a)//判断优先级函数
{
    if(a=='+')
        return 0;
    if(a=='-')
        return 0;
    if(a=='*')
        return 1;
    if(a=='/')
        return 1;
    if(a=='^')
        return 2;
    if(a=='s')
        return 3;
    if(a=='c')
        return 3;
    if(a=='t')
        return 3;
    if(a=='n')
        return 3;
    if(a=='g')
        return 3;
}
QString Calqulator::bin_change(QString s)
{
//    qDebug()<<"bin_change";
//    int times=1;
//    int sum=0;
//    for(int i=s.length()-1;i>=0;i--)
//    {
//        int tempnum=s[i]-'0';
//       sum=sum+tempnum*times;
//        times=times*2;
//    }
//    qDebug()<<sum;
//    QString dec_str;
//    for(;sum>0;)
//    {
//        int mod=sum%10;
//        sum=sum/10;
//        char element=mod+0x30;
//        dec_str.push_front(element);
//    }
//    qDebug()<<dec_str;
//    return dec_str;
}
QString Calqulator::intopost(QString infix) throw (const char*)//throw可能抛出char*异常
{
    std::stack<char>stack;
    char current;//未初始化0
    QString postfix;//后缀数组
    qDebug()<<infix;
    for(int i=0;i<infix.length();i++)
    {
        current=infix[i].toLatin1();//转换为数字或者运算符
        if(isdigit(current))//先将数字放入后缀
        {
            postfix.push_back(current);//依次放入后缀数组
            qDebug()<<"digit: "<<current;
            qDebug()<<postfix;
            continue;
        }
        //将运算符放入后缀
        qDebug()<<"operator: "<<current;
        if(current=='+'||current=='-'||current=='*'||current=='/'||current=='^')//
        {
            if(infix[i-1]!='(')
            {
                if(infix[i-1].isDigit()==true)//数字后放一个#隔开
                {
                    postfix.push_back('#');
                }
                else
                    if(infix[i-1]=='%')
                    {

                    }
                else
                    {
                        throw "Syntax Error";
                    }
            }
            if(stack.empty()==false)
            {
                char temptop=stack.top();
                for(;temptop!='('&&priority(current)<=priority(temptop);)//<还是<=???
                {
                    stack.pop();
                    postfix.push_back(temptop);
                    if(stack.empty())
                    {
                        break;
                    }
                    temptop=stack.top();
                }
            }
            stack.push(current);
            qDebug()<<postfix;
            continue;
        }//+-*/结束,60分内容之一
        if(current=='s'||current=='c'||current=='t')//sin,cos,tan
        {
            if(i>0&&infix[i-1].isDigit()==true)
            {
                throw "Syntax Error";
            }
            if(stack.empty()==false)
            {
                char temtop=stack.top();
                for(;temtop!='('&&priority(current)<priority(temtop);)
                {
                    stack.pop();
                    postfix.push_back(temtop);
                    if(stack.empty()==true)
                    {
                        break;
                    }
                    temtop=stack.top();
                }
            }
            stack.push(current);
            i+=2;
            continue;
        }
        if(current=='l')//ln,lg
        {
            if(i>0&&infix[i-1].isDigit()==true)
            {
                throw "Syntax Error";
            }
            if(stack.empty()==false)
            {
                char temptop=stack.top();
                for(;temptop!='('&&priority(current)<priority(temptop);)
                {
                    stack.pop();
                    postfix.push_back(temptop);
                    if(stack.empty()==true)
                    {
                        break;
                    }
                    temptop=stack.top();
                }
            }
            stack.push(current);
            i++;
            continue;
        }
        if(current=='.')
        {
            postfix.push_back(current);
            continue;
        }
        if(current=='%')
        {
            postfix.push_back('#');
            postfix.push_back(current);
            continue;
        }
        if(current=='(')
        {
            stack.push(current);
            continue;
        }
        if(current==')')
        {
            if(infix[i-1]!='%')
            {
                postfix.push_back('#');
            }
            char temtop;
            temtop=stack.top();
            for(;temtop!='(';)
            {
                stack.pop();
                postfix.push_back(temtop);
                temtop=stack.top();
            }
            stack.pop();
            continue;
        }
        throw "Syntax Error";
        continue;
    }
    if(infix[infix.size()-1]!=')')
    {
        if(infix[infix.size()-1].isDigit())
        {
            postfix.push_back('#');
        }
        else if(infix[infix.size()-1]=='%')
        {

        }
        else
        {
            throw "expression is illegality";
        }
    }
    while(stack.empty()==false)
    {
        char temptop=stack.top();
        stack.pop();
        postfix.push_back(temptop);
    }
    qDebug()<<postfix;
    return postfix;
}
double Calqulator::compute(QString s) throw (const char*)
{
    std::stack<double>stack;
    double outcome;
    double num1;
    double num2;
    QString str;
    for(int i=0;i<s.length();i++)
    {
        if(s[i].isDigit()==true)
        {
            str.push_back(s[i]);
            continue;
        }
        auto oper=s[i].toLatin1();
        if(oper=='+'||oper=='-'||oper=='*'||oper=='/')
        {
            if(oper=='+')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1+num2);
                qDebug()<<num1<<"+"<<num2<<"="<<num1+num2;
                continue;
            }
            if(oper=='-')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num2-num1);
                qDebug()<<num2<<"-"<<num1<<"="<<num2-num1;
                continue;
            }
            if(oper=='*')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1*num2);
                qDebug()<<num1<<"*"<<num2<<"="<<num1*num2;
                continue;
            }
            if(oper=='/')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num2/num1);
                qDebug()<<num2<<"/"<<num1<<"="<<num2/num1;
                continue;
            }
        }
        if(oper=='#')
        {
            if(str.isEmpty())
            continue;
            outcome=str.toDouble();//同时可以将多位数转变为正常数字
            str.clear();
            stack.push(outcome);
            qDebug()<<outcome;
            continue;
        }
        throw "illeaglity";
    }
    outcome=stack.top();
    qDebug()<<outcome;
    return outcome;
}
