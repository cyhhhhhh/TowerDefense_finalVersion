#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <vector>
#include "bullet.h"
#include "target.h"
using namespace std;

class WayPoint;
class QPainter;
class MainWindow;
class Tower;
class TowerBase;
class MyGame;

class EnemyBase : public QObject,public Target
{
     Q_OBJECT
public:
    EnemyBase(WayPoint *startWayPoint, MyGame *game,double speed,const QPixmap &sprite,int maxhp=40);
    ~EnemyBase();
    virtual void draw(QPainter *painter) const;
    virtual void checkAttactingTower();
    void move();
    virtual void getDamage(int damage,bool ice);
    void getRemoved();
    void getAttacked(TowerBase *attacker);
    void getLostSight(TowerBase *attacker);
    virtual void setAttackingTargetNull();
    bool check(Target* target);
    QPoint pos()const;
    vector<TowerBase *> m_attackedTowers;
public slots:
    void doActivate();
protected:
    bool			m_active;
    int				m_maxHp;
    int				m_currentHp;
    bool            m_iced;
    qreal			m_walkingSpeed;
    qreal			m_rotationSprite;

    QPoint			m_pos;
    WayPoint *		m_destinationWayPoint;
    MyGame *	    m_game;
    const QPixmap	m_sprite;
};

class Enemy : public EnemyBase
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MyGame *game, const QPixmap &sprite = QPixmap(":/image/enemy.png"));
    void draw(QPainter *painter) const;
    ~Enemy();
    void checkAttactingTower() ;
    void setAttackingTargetNull() ;
private:
    static const QSize ms_fixedSize;
};

class shootingEnemy : public EnemyBase
{
    Q_OBJECT
public:
    shootingEnemy(WayPoint *startWayPoint, MyGame *game, const QPixmap &sprite = QPixmap(":/image/enemy1.png"),
                  int maxhp=180);
    void checkAttactingTower();
    //bool checkTarget();
    ~shootingEnemy();
    void setAttackingTargetNull();
    void draw(QPainter *painter) const;
protected:
    //int shootingtimes;
    //int maxshootingtimes;
    QTimer * m_fireRateTimer;
    int	     m_fireRate;
    TowerBase* attackingTarget;
    static const QSize ms_fixedSize;
public slots:
    void shootWeapon();
};

class destroyingEnemy : public EnemyBase
{
    Q_OBJECT
public:
    destroyingEnemy(WayPoint *startWayPoint, MyGame *game, const QPixmap &sprite = QPixmap(":/image/destroyingenemy.png"));
    void draw(QPainter *painter) const ;
    void destroy();
    void getDamage(int damage,bool ice) ;
protected:
    int attackingrange;
    static const QSize ms_fixedSize;
};



#endif // ENEMY_H
