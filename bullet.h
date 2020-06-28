#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include "tower.h"
#include "enemy.h"
#include "target.h"
#include "mygame.h"
class QPainter;
class EnemyBase;
class TowerBase;
class MainWindow;
class MyGame;

class BulletBase:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    BulletBase(QPoint startPos, QPoint targetPoint, int damage, Target* target,
               MyGame *game,const QPixmap &sprite, bool ice_fun);
    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;
    virtual bool checkTarget()=0;
protected slots:
    void hitTarget();
protected:
    bool            m_ice;//�Ƿ���б�������
    QPoint	        m_startPos;//�ӵ���ʼλ�ü���������λ��
    QPoint	        m_targetPos;//��ֹλ�ü�����λ��
    QPixmap	        m_sprite;
    QPoint			m_currentPos;//�ӵ���ǰλ��
    Target *		m_target;//Ŀ�����
    MyGame *	    m_game;
    int				m_damage;//���������ɹ���������
};

class Bullet : public BulletBase
{
public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, EnemyBase* target,
           MyGame *game, const QPixmap &sprite,bool ice_fun=false);
    bool checkTarget() ;
private:
    static const QSize ms_fixedSize;
};

class Bullet1 :public BulletBase
{
public:
    Bullet1(QPoint startPos, QPoint targetPoint, int damage, TowerBase* target,
            MyGame *game,const QPixmap &sprite= QPixmap(":/image/bullet1.png"),bool ice_fun=false);
    bool checkTarget() ;
private:
    static const QSize ms_fixedSize;
};



#endif // BULLET_H
