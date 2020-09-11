#ifndef FORM_H
#define FORM_H

#include <QWidget>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();


private:
    Ui::Form *ui;
public:
    void setline(QString s);
    void clear();
    void sendsignal();
    bool ok=true;
signals:
    void slot1();

};

#endif // FORM_H
