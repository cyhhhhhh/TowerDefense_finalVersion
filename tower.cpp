#include "tower.h"
//#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "func_test_coll.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>

const QSize Tower::ms_fixedSize(73, 60);
const QSize TowerBase::ms_fixedSize(73, 60);
void TowerBase::draw(QPainter *painter) const
{
}
void TowerBase::shootWeapon()
{}
void TowerBase::getremoved()
{
    m_game->removeTower(this);
}
void TowerBase::getDamage(int damage,bool ice)
{
    hitedtimes++;
    if(hitedtimes>=3)
    {
        getremoved();
    }
}
/*void TowerBase::beAttacked(EnemyBase *enemy)
{
    attackingEnemy.push_back(enemy);
}*/
void TowerBase::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);
}
void TowerBase::chooseEnemyForAttack(EnemyBase *enemy)
{
    m_chooseEnemy = enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);
}
void TowerBase::targetKilled()
{
    if (m_chooseEnemy)
      {  m_chooseEnemy = NULL;
         m_rotationSprite = 0.0;
    }
}
TowerBase::TowerBase(QPoint pos, MyGame *game,const QPixmap &sprite,
                     const QPixmap &bulletImage,int co,int maxhit,int firerate)
    :m_attackRange(80)
    ,m_damage(10)
    ,m_fireRate(firerate)
    ,cost(co)
    ,hitedtimes(0)
    ,maxHitedTimes(maxhit)
    ,m_rotationSprite(0.0)
    ,m_chooseEnemy(NULL)
    ,m_game(game)
    ,m_pos(pos)
    ,m_sprite(sprite)
    ,m_bulletImage(bulletImage)
{}
void TowerBase::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(m_chooseEnemy->pos() - m_pos);
        normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        // 如果敌人脱离攻击范围
        if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        vector<EnemyBase *> Enemylist = m_game->getEnemies();
        foreach (EnemyBase *enemy, Enemylist)
        {
            if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}
bool TowerBase::check(Target*target)
{
    QList<TowerBase*> Towerlist=m_game->getTowers();
    for(QList<TowerBase*>::iterator iter=Towerlist.begin();iter!=Towerlist.end();iter++)
    {
        if(*iter==target)
            return true;
    }
    return false;
}
QPoint TowerBase::getpos()
{
    return m_pos;
}
void TowerBase::lostSightOfEnemy()
{
    m_chooseEnemy->getLostSight(this);
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();//当敌人走出视野范围，将攻击塔与敌人取消关联，并停止攻击
    m_rotationSprite = 0.0;
}
int TowerBase::knowcost()
{
    return cost;
}
bool TowerBase::containPoint(const QPoint &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

Tower::Tower(QPoint pos, MyGame *game, const QPixmap &sprite,const QPixmap &bulletImage)
    : TowerBase(pos,game,sprite,bulletImage,300,3,1000)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));//将“发射子弹”与“发射频率”关联起来
}

void Tower::draw(QPainter *painter) const
{
    painter->save();

    // 绘制偏转坐标,由中心+偏移=左上
   static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    //绘制炮塔并选择炮塔
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Tower::shootWeapon()
{
    if(m_game->checkEnemy(m_chooseEnemy))
    {
    Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy,
                                m_game, m_bulletImage);
    bullet->move();
    m_game->audioPlayer()->playSound(LaserShootSound);
    m_game->addBullet(bullet);
    }
}
void Tower::update()
{
    m_game->updateTower(this);
}
bool Tower::checkupdate()
{
    return true;
}
const QSize UpdatedTower::ms_fixedSize(73, 60);

UpdatedTower::UpdatedTower(QPoint pos, MyGame *game, const QPixmap &sprite,
                           const QPixmap &bulletImage)
    : TowerBase(pos,game,sprite,bulletImage,300,7,400)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));//将“发射子弹”与“发射频率”关联起来
}
void UpdatedTower::update()
{}
void UpdatedTower::draw(QPainter *painter) const
{
    painter->save();

    // 绘制偏转坐标,由中心+偏移=左上
   static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    //绘制炮塔并选择炮塔
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}
bool UpdatedTower::checkupdate()
{
    return false;
}
void UpdatedTower::shootWeapon()
{
    if(m_game->checkEnemy(m_chooseEnemy))
    {
        Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,
                                    m_bulletImage,true);
        bullet->move();
        m_game->audioPlayer()->playSound(LaserShootSound);
        m_game->addBullet(bullet);
    }
}

