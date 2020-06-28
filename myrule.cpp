#include "myrule.h"
#include "button.h"
MyRule::MyRule(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(800,600);
    button* btn3=new button(":/image/button3.png");
    btn3->setParent(this);
    btn3->move(700,500);
    connect(btn3,&button::clicked,this,[=](){
        emit back();
    });

}
void MyRule::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap  pixmap(":/image/rule.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
}
