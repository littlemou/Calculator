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
    QMessageBox::warning(this,"语法错误","wrong!");

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

    QPushButton *specialoperatorbtn[6]={ui->sinbtn,ui->cosbtn,ui->tanbtn,
                                       ui->lnbtn,ui->lgbtn,ui->e_xbtn};
    for(auto btn:specialoperatorbtn)//ln,lg,sin,cos,tan,e^x
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    connect(ui->backbtn,&QPushButton::clicked,this,&Calqulator::back_clicked);
    connect(ui->allclearbtn,&QPushButton::clicked,this,&Calqulator::allclear_clicked);
    connect(ui->pointbtn,&QPushButton::clicked,this,&Calqulator::point_clicked);
    connect(ui->binbtn,&QPushButton::clicked,this,&Calqulator::binary_clicked);
    connect(ui->hexbtn,&QPushButton::clicked,this,&Calqulator::hex_clicked);
    connect(ui->comparebtn,&QPushButton::clicked,this,&Calqulator::compare_clicked);
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
    if(value=="return")
    {
        ui->lineEdit->setText(ui->lineEdit->text());
    }
    else
    if(waitforoperand==true&&value=="(")
    {
        ui->lineEdit->setText(value);
        waitforoperand=false;
    }
    else
    if(waitforoperand==true&&value=="-")
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
        if(value=="e^x")
        {
            ui->lineEdit->setText(ui->lineEdit->text()+"e^");
        }
        else
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}
void Calqulator::compare_clicked()
{
    QString str=ui->lineEdit->text();
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(str+" "+ui->comparebtn->text()+" ");
    }
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::compare_equal_clicked);
}
void Calqulator::compare_equal_clicked()
{
    for(int )
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
}
void Calqulator::equal_clicked()
{
    double result=0;
    QString value=ui->lineEdit->text();//value之后会是二进制转换之后的字符串
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='b')
        {
            value=bin_change(i,value);//将二进制字符串转换为相应十进制字符串
        }
        if(value[i]=='x')
        {
            value=hex_change(i,value);//将16进制字符转换为相应的十进制字符
        }
    }
    qDebug()<<value;
    //bao错并计算
    try
    {
        result=compute(intopost(value));//转换函数
    }
    catch(const char*er)
    {
        error=er;
        abortoperation();
        return;
    }
    ui->lineEdit->setText((ui->lineEdit->text()+'='+QString::number(result)));
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
    QPushButton *binbtn=static_cast<QPushButton*>(sender());//判断发出对象
    QString value=binbtn->text();
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
void Calqulator::hex_clicked()
{
    QPushButton *hexbtn=static_cast<QPushButton*>(sender());
    QString value=hexbtn->text();
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
    ui->pawbtn->setText("return");
    connect(ui->pawbtn,&QPushButton::clicked,this,&Calqulator::return_clicked);
    ui->lnbtn->setText("A");
    ui->e_xbtn->setText("B");
    ui->lgbtn->setText("C");
    ui->sinbtn->setText("D");
    ui->cosbtn->setText("E");
    ui->tanbtn->setText("F");
}

void Calqulator::return_clicked()
{
    ui->pawbtn->setText("^");
    ui->lnbtn->setText("ln");
    ui->e_xbtn->setText("e^x");
    ui->lgbtn->setText("lg");
    ui->sinbtn->setText("sin");
    ui->cosbtn->setText("cos");
    ui->tanbtn->setText("tan");
}
int Calqulator::priority(char a)//判断//优先级函数
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
QString Calqulator::bin_change(int i,QString value)
{
    int bin=i+1;
    int number;
    if(value[bin]!='0'&&value[bin]!='1')
    {
        value.push_back('+');
        return value;
    }
    for(number=0;value[bin]=='0'||value[bin]=='1';bin++)
    {
        number++;
        if(value[bin+1]>'1'&&value[bin+1]<'9')
        {
            value.push_back('+');
            return value;
        }
    }
    QString str=value.mid(i+1,number);//二进制字符串
    int binary=str.toInt(&ok_change,10);
    int times=1;
    int sum=0;
    while(binary>0)
    {
        int mod=binary%10;
        binary=binary/10;
        sum=sum+mod*times;
        times=times*2;
    }
    QString bin_dec=QString::number(sum,10);
    value=value.replace(i-1,i+1+number,bin_dec);//用bin_dec替换i-1到i+1+number的字符
    return value;
}
QString Calqulator::hex_change(int i,QString value)
{
    int bin=i+1;
    int flag=0;//判断结束
    int number=0;//计算字符串位数
    QString letter="0123456789ABCDEF";
    for(int n=bin;;n++)
    {
        for(int m=0;m<16;m++)
        {
            if(letter[m]==value[n])
            {
                number++;
                break;
            }
            if(m==15)
            flag=1;
        }
        if(flag==1)
            break;
    }
    int times=1;
    int sum=0;
    QString str=value.mid(i+1,number);
    for(int n=str.length()-1;n>=0;n--)
    {
        for(int m=0;m<15;m++)
        {
            if(str[n]==letter[m])
            {
                sum+=m*times;
                times=times*16;
                break;
            }
        }
    }
    QString hex_dec=QString::number(sum,10);//将sum转换为字符串
    value=value.replace(i-1,i+1+number,hex_dec);//用hex_dec替换i-1到i+1+number的字符
    return value;
}
QString Calqulator::intopost(QString infix) throw (const char*)//throw可能抛出char*异常
{
//    infix=point_change(infix);
    std::stack<char>stack;
    char current;//未初始化0
    QString postfix;//后缀数组
    if(infix[0]=='-')
    {
        infix.push_front('0');
    }
    qDebug()<<postfix;
    for(int i=0;i<infix.length();i++)
    {
        current=infix[i].toLatin1();//转换为数字或者运算符
        if(isdigit(current))//将数字放入后缀
        {
            postfix.push_back(current);//依次放入后缀数组
            qDebug()<<postfix;
            continue;
        }
        //将运算符放入后缀
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
                    if(infix[i-1]==')')
                    {
                        stack.push(current);
                        continue;
                    }
                else
                    {
                        throw "Syntax Error";
                    }
               // else
                //if(infix=='')
            }
            if(stack.empty()==false)
            {
                char temptop=stack.top();
                for(;temptop!='('&&priority(current)<=priority(temptop);)//<还是<=???
                {
                    stack.pop();
                    postfix.push_back(temptop);
                    qDebug()<<postfix;
                    if(stack.empty())
                    {
                        break;
                    }
                    temptop=stack.top();
                }
            }
            stack.push(current);
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
            current=infix[i+1].toLatin1();
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
            qDebug()<<"处理反括号";
            if(infix[i-1]!='%')
            {
                postfix.push_back('#');
            }
            qDebug()<<postfix;
            char temtop;
            temtop=stack.top();
            for(;temtop!='(';)
            {
                stack.pop();
                postfix.push_back(temtop);
                qDebug()<<postfix;
                temtop=stack.top();
            }
            stack.pop();
            qDebug()<<"处理反括号";
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
            str.push_back(s[i]);//先将数字存入
            continue;
        }
        auto oper=s[i].toLatin1();
        if(oper=='+'||oper=='-'||oper=='*'||oper=='/'||oper=='^')
        {
            if(oper=='+')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1+num2);
                continue;
            }
            if(oper=='-')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num2-num1);
                continue;
            }
            if(oper=='*')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num1*num2);
                continue;
            }
            if(oper=='/')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(num2/num1);
                continue;
            }
            if(oper=='^')
            {
                num1=stack.top();
                stack.pop();
                num2=stack.top();
                stack.pop();
                stack.push(pow(num2,num1));
                continue;
            }
        }
        if(oper=='s'||oper=='c'||oper=='t')
        {
            if(oper=='s')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::sin(num1/180.0*M_PI));
                continue;
            }
            if(oper=='c')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::cos(num1/180.0*M_PI));
                continue;
            }
            if(oper=='t')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::tan(num1/180.0*M_PI));
                continue;
            }

        }
        if(oper=='n'||oper=='g')
        {
            if(oper=='n')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::log(num1));
                continue;
            }
            if(oper=='g')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::log10(num1));
                continue;
            }
        }
        if(oper=='.')
        {
            str.push_back('.');
            continue;
        }
        if(oper=='#')
        {
            if(str.isEmpty())
            continue;
            outcome=str.toDouble();//同时可以将多位数转变为正常数字
            str.clear();
            stack.push(outcome);
            continue;
        }
        throw "illeaglity";
    }
    outcome=stack.top();
    return outcome;
}
