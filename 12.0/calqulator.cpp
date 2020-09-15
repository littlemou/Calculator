
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
#include "form.h"

Calqulator::Calqulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calqulator)
{
    ui->setupUi(this);
    this->setFixedSize(1300,800);
    this->move(500,1000);
    ui->lineEdit->setText("0");
    waitforoperand=true;//等待计算
    connectslots();
    void(Form::*funslot1)()=&Form::slot1;
    connect(&w,funslot1,this,&Calqulator::allclear_clicked);
    ui->lineEdit->setReadOnly(true);
}

Calqulator::~Calqulator()
{
    delete ui;
}

void Calqulator::abortoperation()//中止函数
{
    ui->lineEdit->setText("0");
    waitforoperand=true;
    QMessageBox::warning(this,"wrong!","语法错误");
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
    connect(ui->hypbtn,&QPushButton::clicked,this,&Calqulator::hyp_clicked);
    connect(ui->conversition,&QPushButton::clicked,this,&Calqulator::sendsignal);
    connect(ui->cs_window,&QPushButton::clicked,this,&Calqulator::cswidow_clicked);
    connect(ui->memory,&QPushButton::clicked,this,&Calqulator::memory_clicked);
}

void Calqulator::digital_clicked()//字符串输入显示 60
{
    qDebug()<<"进如此循环";
    QPushButton *digitbtn=static_cast<QPushButton*>(sender());//判断发出对象????
    QString value=digitbtn->text();
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
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}

void Calqulator::cswidow_clicked()
{
    w.show();
    w.setWindowTitle("Conversion Window");
    w.setFixedSize(400,300);
    w.move(0,300);
}

void Calqulator::sendsignal()
{
    w.setline(ui->lineEdit->text());
}

void Calqulator::memory_clicked()
{
    if(memory=="wrong222")
    {
        ui->lineEdit->setText("0");
        waitforoperand=true;
        QMessageBox::warning(this,"wrong!","无记忆结果");
    }
    else
    {
        ui->lineEdit->setText(memory);
        waitforoperand=true;
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
                memory=ui->lineEdit->text();
                waitforoperand=true;
            }
            else
            if(front_digital==back_digital)
            {
                ui->lineEdit->setText(value+" : "+front_str+"("+front_digital_str+")"+"="+back_str+"("+back_digital_str+")");
                memory=ui->lineEdit->text();
                waitforoperand=true;
            }
            else
            {
                ui->lineEdit->setText(value+" : "+front_str+"("+front_digital_str+")"+"<"+back_str+"("+back_digital_str+")");
                memory=ui->lineEdit->text();
                waitforoperand=true;
            }
            break;
        }
    }
}

void Calqulator::equal_clicked()
{
    double result=0.0;
    QString value=ui->lineEdit->text();//value之后会是二进制转换之后的字符串
    result=compute(intopost(value));//转换函数
    if(flag=="flag")
    {
        qDebug()<<"执行非正常等于号";
        flag="0";
        waitforoperand=true;
        ui->lineEdit->setText("0");
        abortoperation();
        return;
    }
    else
    {
        qDebug()<<"执行正常等于号000";
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
        if(trans=="hex")
        {
            int resulthex=int(result);
            ui->lineEdit->setText(ui->lineEdit->text()+" = "+"0x"+QString::number(resulthex,16));
            memory=ui->lineEdit->text();
            waitforoperand=true;//重置
            return;
        }
        else if(trans=="bin")
        {
            int resultbin=int(result);
            ui->lineEdit->setText(ui->lineEdit->text()+" = "+"0b"+QString::number(resultbin,2));
            memory=ui->lineEdit->text();
            waitforoperand=true;//重置
            return;
        }
        ui->lineEdit->setText(ui->lineEdit->text()+" = "+QString::number(result));
        memory=ui->lineEdit->text();
        waitforoperand=true;//重置
    }
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
    w.clear();
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
            memory=ui->lineEdit->text();
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
    memory=ui->lineEdit->text();
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
            if(value[i+3]=='b'&&value[i+2]!='0')
            {
                abortoperation();
                return;
            }
            for(int j=i+2;value[j]!=' ';j++)
            {
                a_str.push_back(value[j]);
                qDebug()<<a_str;
            }
            a_digital=compute(intopost(a_str));
            qDebug()<<a_digital;
            a_str=QString::number(a_digital,'g',6);
            continue;
        }
        if(value[i]=='b'&&value[i-1]==' ')
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
        memory=ui->lineEdit->text();
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
            memory=ui->lineEdit->text();
        }
        if(trianglein==0)
        {
            ui->lineEdit->setText(value+' -> '+str+"=0"+" x1=x2="+QString::number(outcome1,'g',6));
            memory=ui->lineEdit->text();
        }
        if(trianglein<0)
        {
            ui->lineEdit->setText(value+' -> '+str+"=0"+" x1="+QString::number(front,'g',6)+"+"+QString::number(back,'g',6)+"i,x2="+QString::number(front,'g',6)+"-"+QString::number(back,'g',6)+"i");
            memory=ui->lineEdit->text();
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
    memory=ui->lineEdit->text();
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
    memory=ui->lineEdit->text();
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
    if(ui->sinbtn->text()=="y=sinhx")
    {
        for(auto btn:specialoperatorbtn)//
            connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_2h_clicked);
    }
    else
    {
        for(auto btn:specialoperatorbtn)//
            connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_2_clicked);
    }
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
    if(outcome>1)
    {
        abortoperation();
        return;
    }
    ui->lineEdit->setText(value+" x="+QString::number(outcome,'g',6));
    memory=ui->lineEdit->text();
    waitforoperand=true;
}

void Calqulator::hyp_clicked()
{
    if(ui->sinbtn->text()=="y=sinx")
    {
        abortoperation();
        return;
    }
    disconnect(ui->hypbtn,&QPushButton::clicked,this,&Calqulator::hyp_clicked);
    connect(ui->hypbtn,&QPushButton::clicked,this,&Calqulator::hyp_return_clicked);
    QPushButton *specialoperatorbtn[3]={ui->sinbtn,ui->cosbtn,ui->tanbtn};
    for(int i=0;i<3;i++)
    {
        specialoperatorbtn[i]->setText(specialoperatorbtn[i]->text()+"h");
    }
    for(auto btn:specialoperatorbtn)//sin,cos,tan
    {
        disconnect(btn,0,0,0);
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_h_clicked);
    }
}

void Calqulator::specialoperator_2h_clicked()
{
    QPushButton *digitbtn=static_cast<QPushButton*>(sender());//判断发出对象????
    QString value=digitbtn->text();
    QString value2=ui->lineEdit->text();
    if(waitforoperand==false)
    {
        ui->lineEdit->setText(value2+value.right(value.size()-1));
        disconnect(ui->equalbtn,0,0,0);
        connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::specialoperator_2h_equal_clicked);
        waitforoperand=true;
        ui->equalbtn->setText("FH=");
    }
    else
    {
        abortoperation();
        return;
    }
}

void Calqulator::specialoperator_2h_equal_clicked()
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
            outcome=std::asinh(y_digital);
            break;
        }
        if(value[i]=='c')
        {
            outcome=std::acosh(y_digital);
            break;
        }
        if(value[i]=='t')
        {
            outcome=std::atanh(y_digital);
            break;
        }
    }
    ui->lineEdit->setText(value+" x="+QString::number(outcome,'g',6));
    memory=ui->lineEdit->text();
    waitforoperand=true;
    disconnect(ui->equalbtn,0,0,0);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
}

void Calqulator::hyp_return_clicked()
{
    disconnect(ui->hypbtn,0,0,0);
    connect(ui->hypbtn,&QPushButton::clicked,this,&Calqulator::hyp_clicked);
    disconnect(ui->equalbtn,0,0,0);
    connect(ui->equalbtn,&QPushButton::clicked,this,&Calqulator::equal_clicked);
    QPushButton *specialoperatorbtn[3]={ui->sinbtn,ui->cosbtn,ui->tanbtn};
    for(auto btn:specialoperatorbtn)//sin,cos,tan
    {
        disconnect(btn,0,0,0);
        connect(btn,&QPushButton::clicked,this,&Calqulator::specialoperator_clicked);
    }
    for(int i=0;i<3;i++)
    {
        QString str=specialoperatorbtn[i]->text();
        specialoperatorbtn[i]->setText(specialoperatorbtn[i]->text().left(specialoperatorbtn[i]->text().size()-1));
    }
}

void Calqulator::specialoperator_h_clicked()
{
    QPushButton *specialoperatorbtn=static_cast<QPushButton*>(sender());
    QString value=ui->lineEdit->text();
    if(waitforoperand==true&&value=="0")
    {
        ui->lineEdit->setText(specialoperatorbtn->text());
    }
    else
    {
        ui->lineEdit->setText(value+specialoperatorbtn->text());
    }

    ui->equalbtn->setText("H=");
    waitforoperand=false;
    qDebug()<<"waitforoperand变为false";
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
        flag="flag";
        return value;
    }
    for(number=0;value[bin]=='0'||value[bin]=='1';bin++)
    {
        number++;
        if(value[bin+1]>'1'&&value[bin+1]<'9')
        {
            flag="flag";
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
    value=value.replace(i-1,number+2,bin_dec);//用bin_dec替换i-1到i+1+number的字符
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
        {
            break;
            flag=0;
        }
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
    value.replace(i-1,number+2,hex_dec);//用hex_dec替换i-1到i+1+number的字符
    return value;
}

QString Calqulator::intopost(QString infix)//throw可能抛出char*异常
{
    if(flag=="flag")
    {
        qDebug()<<"ta2";
        return flag;
    }
    int flag1=0;
    int flagbin=0;
    int flaghex=0;
    int flagdec=0;
    for(int i=0;i<infix.length();i++)
    {
        if(infix[i]=='0')
        {
            if(infix[i+1]=='b'||infix[i+1]=='x')
            {
                flag1=1;
                if(infix[i+1]=='b')
                {
                    for(int j=i+2;j<infix.length();j++)
                    {
                        if(infix[j]=='1'||infix[j]=='0')
                        {
                            flagbin=1;
                            qDebug()<<j<<"flagbin=1;";
                            i=j;
                            continue;
                        }
                        else
                        {
                            i=j;
                            break;
                        }
                    }
                }
                else
                if(infix[i+1]=='x')
                {
                    for(int j=i+2;j<infix.length();j++)
                    {
                        if((infix[j]>='0'&&infix[j]<='9')||(infix[j]>='A'&&infix[j]<='F'))
                        {
                            flaghex=1;
                            qDebug()<<j<<"flaghex=1;";
                            i=j;
                            continue;
                        }
                        else
                        {
                            i=j;
                            break;
                        }
                    }
                }
            }
            else
            {
                if(flag1==1)
                {
                    flag1=0;
                    continue;
                }
                else
                {
                    flagdec=1;
                    qDebug()<<i<<"flagdec=1;";
                }
            }
        }
        else
        if(infix[i].isDigit()==true)
        {
           flagdec=1;
           qDebug()<<i<<"外flagdec=1;";
        }
    }
    if(flagbin==1&&flaghex==0&&flagdec==0)
    {
        trans="bin";
    }
    else if(flagbin==0&&flaghex==1&&flagdec==0)
    {
        trans="hex";
    }
    else
    {
        trans="dec";
    }
    std::stack<char>stack;
    char current;//未初始化0
    QString postfix;//后缀数组
    qDebug()<<"初始状态"<<infix;
    QString bin_dec;
    QString hex_dec;
    for(int i=0;i<infix.length();i++)//有二进制等先转换为10进制
    {
        if(infix[i]=='b'&&infix[i-1]=='0')
        {
            infix=bin_change(i,infix);//将二进制字符串转换为相应十进制字符串
        }
    }
    for(int i=0;i<infix.length();i++)//有二进制等先转换为10进制
    {
        if(infix[i]=='x'&&infix[i-1]=='0')
        {
            infix=hex_change(i,infix);//将16进制字符转换为相应的十进制字符
        }
    }
    for(int j=0;j<infix.length();j++)
    {
        if(j==0&&(infix[0]=='-'||infix[0]=='+'))//初始为-号变成（0-x）
        {
            infix.push_front('0');
            continue;
        }
        else
        if((infix[j]=='-'||infix[j]=='+')&&infix[j-1]=='(')//
        {
            int number=1;
            QString str;
            str.push_back('0');
            str.push_back(infix[j]);
            for(int m=j+1;m<infix.length()&&infix[m]!='+'&&infix[m]!='-'&&infix[m]!='*'&&infix[m]!='/';m++)
            {
                number++;
                str.push_back(infix[m]);
            }
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
                if(infix[i+3]=='h')
                {
                    stack.push(current);
                    stack.push('h');
                    i+=3;
                    continue;
                }
                stack.push(current);
                i+=2;
                continue;
        }
        qDebug()<<"最终逆序2"<<postfix;//
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
        qDebug()<<"最终逆序3"<<postfix;//
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
        char next;
        if(current==')'&&stack.empty()==false)
        {
            qDebug()<<"进入";
            if(i+1>infix.length()-1)
            {

                next=infix[i].toLatin1();
            }
            else
            {
                qDebug()<<"最终逆序53"<<postfix;
                next=infix[i+1].toLatin1();
            }
            qDebug()<<infix;
            if(stack.empty()==false&&stack.top()=='(')
            {
                qDebug()<<"ta4";
                flag="flag";
                return flag;
            }
            if(infix[i-1]!='%')
            {
                postfix.push_back('#');
            }
            char temtop;
            temtop=stack.top();
            int flag2=0;
            for(;stack.empty()==false;)
            {
                stack.pop();
                postfix.push_back(temtop);
                if(stack.empty())
                {
                    break;
                }
                temtop=stack.top();
                if(temtop=='(')
                {
                    flag2=1;
                    stack.pop();//将左括号移除
                    if(stack.empty())
                    {
                        break;
                    }
                    else
                    if(priority(stack.top())<priority(next))
                    {
                        stack.push('(');
                        break;
                    }
                    else
                    {
                        temtop=stack.top();//下一个符号
                    }
                }
            }
            qDebug()<<"最终逆序6"<<postfix;
            if(flag2==0)
            {
                qDebug()<<"ta5";
                flag="flag";
                return flag;
            }
            if(stack.empty())
            {
                continue;
            }
            else
            {
                stack.pop();
                continue;
            }
        }
        qDebug()<<"最终逆序7"<<postfix;
    }
    qDebug()<<"最终逆序8"<<postfix;
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
    if(s=="flag")
    {
        waitforoperand=true;
        return 0;
    }
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
        if(oper=='h')
        {
            i++;
            oper=s[i].toLatin1();
            if(oper=='s')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::sinh(num1));
                continue;
            }
            if(oper=='c')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::cosh(num1));
                continue;
            }
            if(oper=='t')
            {
                num1=stack.top();
                stack.pop();
                stack.push(std::tanh(num1));
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
