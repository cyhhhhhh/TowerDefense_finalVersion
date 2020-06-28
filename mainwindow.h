#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QList>
#include<qpainter.h>
#include<vector>
#include<QTimer>
#include"towerposition.h"
#include"tower.h"
#include"waypoint.h"
#include"enemy.h"
#include"bullet.h"
#include"music.h"
#include"button.h"
#include "mygame.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H


