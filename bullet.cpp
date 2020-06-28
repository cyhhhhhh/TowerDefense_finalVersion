#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPropertyAnimation>

void BulletBase::hitTarget()
{
    if (checkTarget()&&m_target)
    {
        m_target->getDamage(m_damage,m_ice);
    }
    m_game->removedBullet(this);
}
BulletBase::BulletBase(QPoint startPos, QPoint targetPoint, int damage, Target* target,
                       MyGame *game, const QPixmap &sprite,bool ice_fun)
    :m_ice(ice_fun)
    ,m_startPos(startPos)
    ,m_targetPos(targetPoint)
    ,m_sprite(sprite)
    ,m_currentPos(startPos)
    ,m_target(target)
    ,m_game(game)
    ,m_damage(damage)
{}
void BulletBase::draw(QPainter *painter) const
{
    painter->drawPixmap(m_currentPos, m_sprite);
}
void BulletBase::move()
{
    // 100�����ڻ��е���
    static const int duration = 100;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(m_startPos);
    animation->setEndValue(m_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));

    animation->start();
}
void BulletBase::setCurrentPos(QPoint pos)
{
    m_currentPos=pos;
}
QPoint BulletBase::currentPos() const
{
    return m_currentPos;
}
bool BulletBase::checkTarget()
{
}

const QSize Bullet::ms_fixedSize(8, 8);

Bullet::Bullet(QPoint startPos, QPoint targetPoint, int damage, EnemyBase* target,
               MyGame *game,const QPixmap &sprite, bool ice_fun)
    :BulletBase(startPos,targetPoint,damage,target,game,sprite,ice_fun)
{}

bool Bullet::checkTarget()
{
    vector<EnemyBase*> Enemylist=m_game->getEnemies();
    for(vector<EnemyBase*>::iterator iter=Enemylist.begin();iter!=Enemylist.end();iter++)
    {
        if(*iter==m_target)
            return true;//�õ����Դ���
    }
    return false;
}
const QSize Bullet1::ms_fixedSize(30,22);

Bullet1::Bullet1(QPoint startPos, QPoint targetPoint, int damage, TowerBase* target,
                 MyGame *game, const QPixmap &sprite,bool ice_fun)
    :BulletBase(startPos,targetPoint,damage,target,game,sprite,ice_fun)
{}
bool Bullet1::checkTarget()
{
    QList<TowerBase*> Towerlist=m_game->getTowers();
    for(QList<TowerBase*>::iterator iter=Towerlist.begin();iter!=Towerlist.end();iter++)
    {
        if(*iter==m_target)
            return true;
    }
    return false;
}
