#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"towerposition.h"
#include<QMouseEvent>
#include"music.h"
#include"mygame.h"
#include"myrule.h"
#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    this->setFixedSize(800,600);
    ui->setupUi(this);
    button* btn1=new button(":/image/button1.png");
    button* btn2=new button(":/image/button2.png");
    btn1->setParent(this);
    btn2->setParent(this);
    btn1->move(266,270);
    btn2->move(266,380);
    MyRule* rule=new MyRule;
    connect(btn1,&button::clicked,this,[=](){
        MyGame* scene=new MyGame;
        this->hide();
        scene->show();
    });
    connect(btn2,&button::clicked,this,[=](){
        this->hide();
        rule->show();
    });
    connect(rule,&MyRule::back,this,[=](){
        rule->hide();
        this->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap  pixmap(":/image/Scene.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pixmap);
}
