#ifndef MYRULE_H
#define MYRULE_H

#include <QMainWindow>

class MyRule : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyRule(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
signals:
    void back();
};

#endif // MYRULE_H
