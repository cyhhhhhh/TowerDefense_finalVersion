#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <vector>
#include "enemy.h"
#include "bullet.h"
#include "target.h"
#include "mygame.h"
class QPainter;
class Enemy;
class EnemyBase;
class MainWindow;
class QTimer;
class MyGame;
class TowerBase:public QObject,public Target
{
    Q_OBJECT
public:
    TowerBase(QPoint pos, MyGame *game,const QPixmap &sprite,const QPixmap &bulletImage,
              int co,int maxhit,int firerate);
    virtual void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    bool containPoint(const QPoint &pos)const;
    void attackEnemy();
    bool check(Target*target) ;
    void chooseEnemyForAttack(EnemyBase* enemy);
    QPoint getpos();
    void lostSightOfEnemy();
    int  knowcost();
    void getDamage(int damage,bool ice);
    void getremoved();
    virtual bool checkupdate()=0;
    virtual void update()=0;
    void beAttacked(EnemyBase* enemy);
protected slots:
    virtual void    shootWeapon();

protected:
    bool			m_attacking;
    int				m_attackRange;//代表塔可以攻击到敌人的距离
    int				m_damage;//代表攻击敌人时造成的伤害
    int				m_fireRate;//代表再次攻击敌人的时间间隔
    int             cost;//购买该塔所需的金币
    int             hitedtimes;
    int             maxHitedTimes;
    qreal			m_rotationSprite;

    EnemyBase*      m_chooseEnemy;
    MyGame*	        m_game;
    QTimer *		m_fireRateTimer;

    const QPoint	m_pos;
    const QPixmap	m_sprite;
    const QPixmap   m_bulletImage;
    static const QSize ms_fixedSize;
};

class Tower :public  TowerBase
{
    Q_OBJECT
public:
    Tower(QPoint pos, MyGame *game, const QPixmap &sprite = QPixmap(":/image/tower.png"),
            const QPixmap &bulletImage=QPixmap(":/image/bullet.png"));
    bool checkupdate();
    void draw(QPainter *painter) const;
    void update();
    static const QSize ms_fixedSize;
protected slots:
    void shootWeapon();
};

class UpdatedTower: public TowerBase
{
    Q_OBJECT
public:
    UpdatedTower(QPoint pos, MyGame *game, const QPixmap &sprite = QPixmap(":/image/updatedtower.png"),
                 const QPixmap &bulletImage=QPixmap(":/image/ice_bullet.png"));
    bool checkupdate() ;
    void update() ;
    void draw(QPainter *painter) const;
protected slots:
    void shootWeapon();
protected:
    static const QSize ms_fixedSize;

};


#endif // TOWER_H
