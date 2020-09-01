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
    setshortcutkeys();//设置快捷键
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
                                ,ui->modbtn,ui->pawbtn};
    for(auto btn:operatorbtn)//加减乘除括号，取模，次方
        connect(btn,&QPushButton::clicked,this,&Calqulator::operator_clicked);

    QPushButton *specialoperatorbtn[5]={ui->sinbtn,ui->cosbtn,ui->tanbtn,
                                       ui->lnbtn,ui->lgbtn};
    for(auto btn:specialoperatorbtn)//ln,lg,sin,cos,tan
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);


}
void Calqulator::setshortcutkeys()
{

}

void Calqulator::digital_clicked()//字符串输入显示
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

void Calqulator::operator_clicked()
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
    double result=0.0;
    try
    {
        result=compute(intopost(ui->lineEdit->text()));//自写函数
    }
    catch(const char *er)
    {
        error=er;
        abortoperation();
        return;

    }
    ui->lineEdit->setText((ui->lineEdit->text()+'='+QString::number(result)));
    log=ui->lineEdit->text();//string类
    waitforoperand=true;//重置
}

void Calqulator::back_clicked()
{
    QString value=ui->lineEdit->text();
    if(value=="0"||waitforoperand==true)
    {
        ui->lineEdit->setText("0");
        waitforoperand=true;
    }
    else
    {
        ui->lineEdit->setText(value.left(value.count()-1));//左数减一位输出
    }
}

void Calqulator::allclear_clicked()
{
    ui->lineEdit->setText("0");
    waitforoperand=true;
}

void Calqulator::sign_clicked()
{
    QString value=ui->lineEdit->text();
    QChar sign=value[value.size()-1];
}

