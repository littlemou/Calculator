#include "form.h"
#include "ui_form.h"
#include "calqulator.h"
#include <QMessageBox>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}
Form::~Form()
{
    delete ui;
}
void Form::setline(QString s)
{
    if(s[1]=='b')
    {
        for(int bin=2;bin<s.length();bin++)
        {
            if(s[bin]>'1'&&s[bin]<'9')
            {
                QMessageBox::warning(this,"语法错误","wrong!");
                ui->lineEdit1->setText("十进制:");
                ui->lineEdit2->setText("二进制:");
                ui->lineEdit3->setText("十六进制:");
                sendsignal();
                return;
            }
        }
        s=s.right(s.size()-2);
        QString value1=ui->lineEdit1->text();
        QString value2=ui->lineEdit2->text();
        QString value3=ui->lineEdit3->text();
        int s_digital=s.toInt(&ok,2);
        ui->lineEdit1->setText(value1+QString::number(s_digital,10));
        ui->lineEdit2->setText(value1+"0b"+s);
        ui->lineEdit3->setText(value1+"0x"+QString::number(s_digital,16));
    }
    else
    if(s[1]=='x')
    {
        s=s.right(s.size()-2);
        QString value1=ui->lineEdit1->text();
        QString value2=ui->lineEdit2->text();
        QString value3=ui->lineEdit3->text();
        int s_digital=s.toInt(&ok,16);
        ui->lineEdit1->setText(value1+QString::number(s_digital,10));
        ui->lineEdit2->setText(value1+"0b"+QString::number(s_digital,2));
        ui->lineEdit3->setText(value1+"0x"+s);

    }
    else
    {
        QString value1=ui->lineEdit1->text();
        QString value2=ui->lineEdit2->text();
        QString value3=ui->lineEdit3->text();
        int s_digital=s.toInt(&ok,10);
        ui->lineEdit1->setText(value1+s);
        ui->lineEdit2->setText(value2+"0b"+QString::number(s_digital,2));
        ui->lineEdit3->setText(value3+"0x"+QString::number(s_digital,16));
    }
}
void Form::clear()
{
    ui->lineEdit1->setText("十进制:");
    ui->lineEdit2->setText("二进制:");
    ui->lineEdit3->setText("十六进制:");
}
void Form::sendsignal()
{
    emit slot1();
}
