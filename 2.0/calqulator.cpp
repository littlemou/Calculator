#include "calqulator.h"
#include "ui_calqulator.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QClipboard>
#include <stack>
#include <cmath>
#include <qmath.h>
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
    ui->equalbtn->setText("=");
    disconnect(ui->equalbtn,0,0,0);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    disconnect(ui->function_2,0,0,0);
    connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::afunction_2_clicked);
    disconnect(ui->function_a_x,0,0,0);
    connect(ui->function_a_x,&QPushButton::clicked,this,&Calqulator::yfunction_a_x_clicked);
    disconnect(ui->inversefunction,0,0,0);
    connect(ui->inversefunction,&QPushButton::clicked,this,&Calqulator::yinversefunction_clicked);
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
    for(auto btn:specialoperatorbtn)//ln,lg,sin,cos,tan,e^x
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);
    connect(ui->backbtn,&QPushButton::clicked,this,&Calqulator::back_clicked);
    connect(ui->allclearbtn,&QPushButton::clicked,this,&Calqulator::allclear_clicked);
    connect(ui->pointbtn,&QPushButton::clicked,this,&Calqulator::point_clicked);
    connect(ui->binbtn,&QPushButton::clicked,this,&Calqulator::binary_clicked);
    connect(ui->hexbtn,&QPushButton::clicked,this,&Calqulator::hex_clicked);
    connect(ui->comparebtn,&QPushButton::clicked,this,&Calqulator::compare_clicked);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    connect(ui->cdbtn,&QPushButton::clicked,this,&Calqulator::common_divisorbtn_clicked);
    connect(ui->lcmbtn,&QPushButton::clicked,this,&Calqulator::least_common_multiple_clicked);
    connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::afunction_2_clicked);
    connect(ui->function_a_x,&QPushButton::clicked,this,&Calqulator::yfunction_a_x_clicked);
    connect(ui->inversefunction,&QPushButton::clicked,this,&Calqulator::yinversefunction_clicked);
    connect(ui->tofunction,&QPushButton::clicked,this,&Calqulator::tofunction_clicked);
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
    disconnect(ui->equalbtn,0,0,0);
    ui->equalbtn->setText("cmp=");
    QString str=ui->lineEdit->text();
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(str+" "+ui->comparebtn->text()+" ");
    }
    else
    {
        abortoperation();
        return;
    }
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::compare_equal_clicked);//接入新信号
}
void Calqulator::compare_equal_clicked()
{

    disconnect(ui->equalbtn,0,0,0);
    ui->equalbtn->setText("=");
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    QString value=ui->lineEdit->text();
    QString front_str;
    QString back_str;
    double front_digital;
    double back_digital;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='p')
        {
            for(int f=i-5;f>=0;f--)//将前半段提取出来
            {
                front_str.push_front(value[f]);
            }
            try
            {
                front_digital=compute(intopost(front_str));
            }
            catch (const char*er)
            {
                error=er;
                abortoperation();
                return;
            }
            QString front_digital_str=QString::number(front_digital,'g',6);
            for(int b=i+5;b<value.length();b++)//将后半段提取出来
            {
                back_str.push_back(value[b]);
            }
            try
            {
                back_digital=compute(intopost(back_str));
            }
            catch (const char*er)
            {
                error=er;
                abortoperation();
                return;
            }
            QString back_digital_str=QString::number(back_digital,'g',6);
            if(front_digital>back_digital)
            {
                ui->lineEdit->setText(value+" : "+front_str+"("+front_digital_str+")"+">"+back_str+"("+back_digital_str+")");
            }
            else
            if(front_digital==back_digital)
            {
                ui->lineEdit->setText(value+" : "+front_str+"("+front_digital_str+")"+"="+back_str+"("+back_digital_str+")");
                waitforoperand=true;
            }
            else
            {
                ui->lineEdit->setText(value+" : "+front_str+"("+front_digital_str+")"+"<"+back_str+"("+back_digital_str+")");
                waitforoperand=true;
            }
            break;
        }
    }
}
void Calqulator::equal_clicked()
{
    double result=0;
    QString value=ui->lineEdit->text();//value之后会是二进制转换之后的字符串
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
    ui->equalbtn->setText("=");
    disconnect(ui->equalbtn,0,0,0);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    disconnect(ui->function_2,0,0,0);
    connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::afunction_2_clicked);
    disconnect(ui->function_a_x,0,0,0);
    connect(ui->function_a_x,&QPushButton::clicked,this,&Calqulator::yfunction_a_x_clicked);
    disconnect(ui->inversefunction,0,0,0);
    connect(ui->inversefunction,&QPushButton::clicked,this,&Calqulator::yinversefunction_clicked);
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
    disconnect(ui->pawbtn,0,0,0);
    connect(ui->pawbtn,&QPushButton::clicked,this,&Calqulator::return_clicked);
    ui->lnbtn->setText("A");
    ui->tofunction->setText("B");
    disconnect(ui->tofunction,0,0,0);
    connect(ui->tofunction,&QPushButton::clicked,this,&Calqulator::tofunction_B_clicked);
    ui->lgbtn->setText("C");
    ui->sinbtn->setText("D");
    ui->cosbtn->setText("E");
    ui->tanbtn->setText("F");
}
void Calqulator::tofunction_B_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"B");
}
void Calqulator::return_clicked()
{
    disconnect(ui->pawbtn,0,0,0);
    ui->pawbtn->setText("^");
    ui->lnbtn->setText("ln");
    ui->tofunction->setText("ToFunction");
    disconnect(ui->tofunction,0,0,0);
    connect(ui->tofunction,&QPushButton::clicked,this,&Calqulator::tofunction_clicked);
    ui->lgbtn->setText("lg");
    ui->sinbtn->setText("sin");
    ui->cosbtn->setText("cos");
    ui->tanbtn->setText("tan");
    connect(ui->pawbtn,&QPushButton::clicked,this,&Calqulator::operator_clicked);
}
void Calqulator::common_divisorbtn_clicked()
{
    disconnect(ui->equalbtn,0,0,0);
    ui->equalbtn->setText("CD=");
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::common_divisorbtn_equal_clicked);
    QString str=ui->lineEdit->text();
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(str+" cdv ");
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::common_divisorbtn_equal_clicked()
{
    disconnect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::common_divisorbtn_equal_clicked);
    ui->equalbtn->setText("=");
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    QString value=ui->lineEdit->text();
    QString front_str;
    QString back_str;
    int front_digital;
    int back_digital;
    QString outcome;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='d')
        {
            for(int f=i-3;f>=0;f--)
            {
                front_str.push_front(value[f]);
            }
            front_digital=compute(intopost(front_str));
            for(int b=i+3;b<value.length();b++)
            {
                back_str.push_back(value[b]);
            }
            back_digital=compute(intopost(back_str));
            for(int i=1;i<=qMin(front_digital,back_digital);i++)
            {
                if(front_digital%i==0&&back_digital%i==0)
                {
                    outcome.push_back(QString::number(i,10));
                    outcome.push_back(",");
                }
            }
            if(outcome[outcome.length()-1]==',')
            {
                outcome=outcome.left(outcome.size()-1);
            }
            ui->lineEdit->setText(value+" : ("+QString::number(front_digital,10)+")("+QString::number(back_digital,10)+")"+outcome);
            waitforoperand=true;
            break;
        }
    }
}
void Calqulator::least_common_multiple_clicked()
{
    disconnect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    ui->equalbtn->setText("LCM=");
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::least_common_multiple_equal_clicked);
    QString value=ui->lineEdit->text();
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value+" lcm ");
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::least_common_multiple_equal_clicked()
{
    disconnect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::least_common_multiple_equal_clicked);
    ui->equalbtn->setText("=");
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    QString value=ui->lineEdit->text();
    qDebug()<<value;
    QString front_str;
    QString back_str;
    int front_digital;
    int back_digital;
    QString outcome;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='m')
        {
            for(int f=i-4;f>=0;f--)
            {
                front_str.push_front(value[f]);
            }
            front_digital=compute(intopost(front_str));
            for(int b=i+2;b<value.length();b++)
            {
                back_str.push_back(value[b]);
            }
            back_digital=compute(intopost(back_str));
            for(int m=qMax(front_digital,back_digital);;m++)
            {

                if(m%front_digital==0&&m%back_digital==0)
                {
                    outcome.push_back(QString::number(m,10));
                    break;
                }
            }
            break;
        }
    }
    ui->lineEdit->setText(value+" : ("+QString::number(front_digital,10)+")lcm("+QString::number(back_digital)+"): "+outcome);
    waitforoperand=true;
}
void Calqulator::afunction_2_clicked()
{
    if(waitforoperand==true&&ui->lineEdit->text()=="0")
    {
        disconnect(ui->function_2,&QPushButton::clicked,this,&Calqulator::afunction_2_clicked);
        disconnect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
        ui->equalbtn->setText("F2=");
        connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::function_2_equal_clidcked);
        ui->lineEdit->setText("a:");
        waitforoperand=false;
        connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::bfunction_2_clicked);
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::bfunction_2_clicked()
{
    QString value=ui->lineEdit->text();
    disconnect(ui->function_2,&QPushButton::clicked,this,&Calqulator::bfunction_2_clicked);
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value+" b:");
        connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::cfunction_2_clicked);
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::cfunction_2_clicked()
{
    QString value=ui->lineEdit->text();
    disconnect(ui->function_2,&QPushButton::clicked,this,&Calqulator::cfunction_2_clicked);
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value+" c:");
        connect(ui->function_2,&QPushButton::clicked,this,&Calqulator::afunction_2_clicked);
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::function_2_equal_clidcked()
{
    QString value=ui->lineEdit->text();
    QString a_str;
    QString b_str;
    QString c_str;
    double a_digital;
    double b_digital;
    double c_digital;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='a')
        {
            if(value[i+3]=='b')
            {
                abortoperation();
                return;
            }
            for(int j=i+2;value[j]!=' ';j++)
            {
                a_str.push_back(value[j]);
            }
            a_digital=compute(intopost(a_str));
            a_str=QString::number(a_digital,'g',6);
            continue;
        }
        if(value[i]=='b')
        {
            if(value[i+3]=='c')
            {
                abortoperation();
                return;
            }
            for(int j=i+2;value[j]!=' ';j++)
            {
                b_str.push_back(value[j]);
            }
            b_digital=compute(intopost(b_str));
            b_str=QString::number(b_digital,'g',6);
            if(b_digital>0)
            {
                b_str.push_front('+');
            }
            continue;
        }
        if(value[i]=='c')
        {
            for(int j=i+2;j<value.length();j++)
            {
                c_str.push_back(value[j]);
            }
            c_digital=compute(intopost(c_str));
            c_str=QString::number(c_digital,'g',6);
            if(c_digital>0)
            {
                c_str.push_front('+');
            }
            continue;
        }
    }
    if(a_digital==0)
    {
        if(b_digital>0)
        b_str=b_str.right(b_str.size()-1);
        double outcome=(-1)*c_digital/b_digital;
        ui->lineEdit->setText(value+" -> "+b_str+"x"+c_str+"=0"+" x="+QString::number(outcome,'g',6));
    }
    else
    {
        double trianglein=b_digital*b_digital-4*a_digital*c_digital;
        double front=(-1)*b_digital/(2*a_digital);
        double back=sqrt(fabs(trianglein))/(2*a_digital);
        double outcome1=front+back;
        double outcome2=front-back;
        QString out_str;
        QString str=a_str+"x^2"+b_str+"x"+c_str;
        if(trianglein>0)
        {

            ui->lineEdit->setText(value+' -> '+str+"=0"+" x1="+QString::number(outcome1,'g',6)+",x2="+QString::number(outcome2,'g',6));
        }
        if(trianglein==0)
        {
            ui->lineEdit->setText(value+' -> '+str+"=0"+" x1=x2="+QString::number(outcome1,'g',6));
        }
        if(trianglein<0)
        {
            ui->lineEdit->setText(value+' -> '+str+"=0"+" x1="+QString::number(front,'g',6)+"+"+QString::number(back,'g',6)+"i,x2="+QString::number(front,'g',6)+"-"+QString::number(back,'g',6)+"i");
        }
    }
    waitforoperand=true;
    disconnect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::function_2_equal_clidcked);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    ui->equalbtn->setText("=");
}
void Calqulator::yfunction_a_x_clicked()
{
    if(waitforoperand==true&&ui->lineEdit->text()=="0")
    {
        disconnect(ui->function_a_x,0,0,0);
        disconnect(ui->equalbtn,0,0,0);
        connect(ui->function_a_x,&QPushButton::clicked,this,&Calqulator::afunction_a_x_clicked);
        connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::function_a_x_equal_clicked);
        ui->equalbtn->setText("Fa=");
        ui->lineEdit->setText("y:");
        waitforoperand=false;
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::afunction_a_x_clicked()
{
    QString value=ui->lineEdit->text();
    disconnect(ui->function_a_x,0,0,0);
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value+" base:");
        connect(ui->function_a_x,&QPushButton::clicked,this,&Calqulator::yfunction_a_x_clicked);
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::function_a_x_equal_clicked()
{
    QString value=ui->lineEdit->text();
    QString y_str;
    QString base_str;
    double y_digital;
    double base_digital;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='y')
        {
            for(int j=i+2;value[j]!=' ';j++)
            {
                y_str.push_back(value[j]);
            }
            y_digital=compute(intopost(y_str));
        }
        if(value[i]=='e')
        {
            for(int j=i+2;j<value.length();j++)
            {
                base_str.push_back(value[j]);
            }
            base_digital=compute(intopost(base_str));
        }
    }
    if(y_digital==0||base_digital==0)
    {
        abortoperation();
        return;
    }
    double lny_digital=std::log(y_digital);
    double lnbase_digital=std::log(base_digital);
    ui->lineEdit->setText(value+" -> "+y_str+"="+base_str+"^x"+" x="+QString::number(lny_digital/lnbase_digital,'g',6));
    waitforoperand=true;
    disconnect(ui->equalbtn,0,0,0);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    ui->equalbtn->setText("=");
}
void Calqulator::yinversefunction_clicked()
{
    if(waitforoperand==true&&ui->lineEdit->text()=="0")
    {
        disconnect(ui->inversefunction,0,0,0);
        disconnect(ui->equalbtn,0,0,0);
        connect(ui->inversefunction,&QPushButton::clicked,this,&Calqulator::kinversefunction_clicked);
        connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::inversefunction_equal_clicked);
        ui->equalbtn->setText("Fi=");
        ui->lineEdit->setText("y:");
        waitforoperand=false;
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::kinversefunction_clicked()
{
    QString value=ui->lineEdit->text();
    disconnect(ui->inversefunction,0,0,0);
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value+" k:");
        connect(ui->inversefunction,&QPushButton::clicked,this,&Calqulator::yinversefunction_clicked);
    }
    else
    {
        abortoperation();
        return;
    }
}
void Calqulator::inversefunction_equal_clicked()
{
    QString value=ui->lineEdit->text();
    QString y_str;
    QString k_str;
    double y_digital;
    double k_digital;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='y')
        {
            for(int j=i+2;value[j]!=' ';j++)
            {
                y_str.push_back(value[j]);
            }
            y_digital=compute(intopost(y_str));
        }
        if(value[i]=='k')
        {
            for(int j=i+2;j<value.length();j++)
            {
                k_str.push_back(value[j]);
            }
            k_digital=compute(intopost(k_str));
        }
    }
    if(y_digital==0||k_digital==0)
    {
        abortoperation();
        return;
    }
    ui->lineEdit->setText(value+" -> "+QString::number(y_digital,'g',6)+"="+QString::number(k_digital,'g',6)+"/x"+" x="+QString::number(k_digital/y_digital,'g',6));
}
void Calqulator::tofunction_clicked()
{
    disconnect(ui->tofunction,0,0,0);
    connect(ui->tofunction,&QPushButton::clicked,this,&Calqulator::return_tofunction_clicked);
    ui->tofunction->setText("ReturnFun");
    QPushButton *specialoperatorbtn[5]={ui->sinbtn,ui->cosbtn,ui->tanbtn,
                                       ui->lnbtn,ui->lgbtn};
    for(int i=0;i<5;i++)
    {
        specialoperatorbtn[i]->setText("y="+specialoperatorbtn[i]->text()+"x");
    }
    for(auto btn:specialoperatorbtn)//
        disconnect(btn,0,0,0);
    for(auto btn:specialoperatorbtn)//
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_2_clicked);
}
void Calqulator::return_tofunction_clicked()
{
    disconnect(ui->tofunction,0,0,0);
    connect(ui->tofunction,&QPushButton::clicked,this,&Calqulator::tofunction_clicked);
    ui->tofunction->setText("ToFunction");
    QPushButton *specialoperatorbtn[5]={ui->sinbtn,ui->cosbtn,ui->tanbtn,
                                       ui->lnbtn,ui->lgbtn};
    for(int i=0;i<5;i++)
    {
        QString value=specialoperatorbtn[i]->text();
        value=value.right(value.size()-2);
        value=value.left(value.size()-1);
        specialoperatorbtn[i]->setText(value);
    }
    for(auto btn:specialoperatorbtn)//
        disconnect(btn,0,0,0);
    for(auto btn:specialoperatorbtn)//ln,lg,sin,cos,tan,e^x
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);
}
void Calqulator::specialoperator_2_clicked()
{
    QPushButton *specialoperatorbtn=static_cast<QPushButton*>(sender());
    QString value=specialoperatorbtn->text();
    value=value.right(value.size()-1);
    if(waitforoperand==true)
    {
        abortoperation();
        return;
    }
    else
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);
        disconnect(ui->equalbtn,0,0,0);
        connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::specialoperator_2_equal_clicked);
        ui->equalbtn->setText("F=");
    }
}
void Calqulator::specialoperator_2_equal_clicked()
{
    QString value=ui->lineEdit->text();
    QString y_str;
    double y_digital;
    int numi;
    double outcome;
    for(int i=0;i<value.length();i++)
    {
        if(value[i]=='=')
        {
            numi=i;
            for(int j=i-1;j>=0;j--)
            {
                y_str.push_front(value[j]);
            }
            y_digital=compute(intopost(y_str));
            break;
        }
    }
    for(int i=numi+1;i<value.length();i++)
    {
        if(value[i]=='s')
        {
            outcome=std::asin(y_digital);
            break;
        }
        if(value[i]=='c')
        {
            outcome=std::acos(y_digital);
            break;
        }
        if(value[i]=='t')
        {
            outcome=std::atan(y_digital);
            break;
        }
        if(value[i]=='n')
        {
            outcome=qExp(y_digital);
            break;
        }
        if(value[i]=='g')
        {
            outcome=pow(10,y_digital);
            break;
        }
    }
    ui->lineEdit->setText(value+" x="+QString::number(outcome,'g',6));
    waitforoperand=true;
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
QString Calqulator::intopost(QString infix)//throw可能抛出char*异常
{
    std::stack<char>stack;
    char current;//未初始化0
    QString postfix;//后缀数组
    for(int i=0;i<infix.length();i++)
    {
        if(infix[i]=='b')
        {
            infix=bin_change(i,infix);//将二进制字符串转换为相应十进制字符串
        }
        if(infix[i]=='x')
        {
            infix=hex_change(i,infix);//将16进制字符转换为相应的十进制字符
        }
    }
    for(int j=0;j<infix.length();j++)
    {
        if(j==0&&(infix[0]=='-'||infix[0]=='+'))
        {
            infix.push_front('0');
            continue;
        }
        else
        if((infix[j]=='-'||infix[j]=='+')&&(infix[j-1]<'0'||infix[j-1]>'9'))
        {
            int number=4;
            QString str;
            str.push_back('(');
            str.push_back('0');
            str.push_back(infix[j]);
            for(int m=j+1;m<infix.length()&&infix[m]!='+'&&infix[m]!='-'&&infix[m]!='*'&&infix[m]!='/';m++)
            {
                number++;
                str.push_back(infix[m]);
            }
            str.push_back(')');
            infix.replace(j,number,str);
            j=j+number-1;
        }
    }
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
        if(current=='+'||current=='-'||current=='*'||current=='/'||current=='^'||current=='%')//
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
    return postfix;
}
double Calqulator::compute(QString s)
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
        if(oper=='+'||oper=='-'||oper=='*'||oper=='/'||oper=='^'||oper=='%'||oper=='.')
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
            if(oper=='%')
            {
                num1=stack.top();
                stack.pop();
                stack.push(num1/10);
                continue;
            }
            if(oper=='.')
            {
                str.push_back('.');
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
