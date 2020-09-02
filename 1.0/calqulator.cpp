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
}
void Calqulator::setshortcutkeys()
{

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
    double result=0.0;
    try//捕捉异常
    {
        result=compute(intopost(ui->lineEdit->text()));//自写函数
    }
    catch(const char *er)//捕捉异常
    {
        error=er;
        abortoperation();//中断
        return;

    }
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

QString Calqulator::intopost(QString infix) throw (const char*)//throw可能抛出char*异常
{
    std::stack<char>stack;
    char current;//未初始化0
    QString postfix;//后缀数组
    std::map<char,int> priority;//设置优先级
    priority['+']=0;
    priority['-']=0;
    priority['*']=1;
    priority['/']=1;//60
    priority['^']=2;
    priority['s']=3;
    priority['c']=3;
    priority['t']=3;
    priority['n']=3;
    priority['g']=3;

    for(int i=0;i<infix.length();i++)
    {
        current=infix[i].toLatin1();//转换为数字或者运算符
        if(isdigit(current))//先将数字放入后缀
        {
            postfix.push_back(current);//依次放入后缀数组
            continue;
        }
        //将运算符放入后缀
        if(current=='+'||current=='-'||current=='*'||current=='/'||current=='^')//
        {
            if(infix[i-1]!='(')
            {
                if(infix[i-1].isDigit()==true)//判断最后一个字符是否合法
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
                for(;temptop!='('&&priority[current]<priority[temptop];)//
                {
                    stack.pop();
                    postfix.push_back(temptop);
                    if(stack.empty()==false)
                    {
                        break;
                    }
                    temptop=stack.top();
                }

            }
            stack.push(current);
            continue;
        }//+-*/结束,60分内容之一
        //
        if(current=='s'||current=='c'||current=='t')
        {
            if(i>0&&infix[i-1].isDigit()==true)
            {
                throw "Syntax Error";
            }
            if(stack.empty()==false)
            {
                char temtop=stack.top();
                for(;temtop!='('&&priority[current]<priority[temtop];)
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
        }//sin\cos\tan结束
        if(current=='l')
        {
            if(i>0&&infix[i-1].isDigit()==true)
            {
                throw "Syntax Error";
            }
            if(stack.empty()==false)
            {
                char temptop=stack.top();
                for(;temptop!='('&&priority[current]<priority[temptop];)
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
        }//ln\lg结束
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
                postfix.push_back('#');
            else if(infix[infix.size()-1]=='%')
            {}
            else
                throw "expression is illegality";

        }
        while(stack.empty()==false)
        {
            char tempOut=stack.top();
            stack.pop();
            postfix.push_back(tempOut);
        }

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
            }
            if(oper=='-')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1-num2);
            }
            if(oper=='*')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1*num2);
            }
            if(oper=='/')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1/num2);
            }
        }
        throw "illeaglity";

    }
    outcome=stack.top();
    return outcome;
}
