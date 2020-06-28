#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include <QPropertyAnimation>
#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "func_test_coll.h"
#include "mainwindow.h"
static const int Health_Bar_Width = 50;

void EnemyBase::draw(QPainter *painter) const
{}
EnemyBase::EnemyBase(WayPoint *startWayPoint, MyGame *game,double speed,const QPixmap &sprite,int maxhp)
    :QObject(0)
    ,m_active(false)
    ,m_maxHp(maxhp)
    ,m_currentHp(maxhp)
    ,m_iced(false)
    ,m_walkingSpeed(speed)
    ,m_rotationSprite(0.0)
    ,m_pos(startWayPoint->pos())
    ,m_destinationWayPoint(startWayPoint->nextWayPoint())
    ,m_game(game)
    ,m_sprite(sprite)

{

}
EnemyBase::~EnemyBase()
{
     m_attackedTowers.clear();
     m_destinationWayPoint = NULL;
     m_game = NULL;
}
void EnemyBase::move()
{
    if (!m_active)
        return;

    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // 敌人抵达了一个航点
        if (m_destinationWayPoint->nextWayPoint())
        {
            // 还有下一个航点
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {
            // 表示进入基地
            m_game->getHpDamage();
            m_game->removeEnemy(this);
            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPoint targetPoint = m_destinationWayPoint->pos();
    // 未来修改这个可以添加移动状态,加快,减慢,m_walkingSpeed是基准值

    // 向量标准化
    qreal movementSpeed = m_walkingSpeed;
    QVector2D normalized(targetPoint - m_pos);
    normalized.normalize();//将当前位点与目标位点的差向量计算出来，并标准化为单位向量
    m_pos = m_pos + normalized.toPoint() * movementSpeed;

    // 确定敌人选择方向
    // 默认图片向左,需要修正180度转右
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}
void EnemyBase::getDamage(int damage,bool ice)
{
    m_currentHp -= damage;
    // 阵亡,需要移除
    if(ice)
    {
        m_walkingSpeed=0.6;
        m_iced=true;
    }
    if (m_currentHp <= 0)
    {
        m_game->audioPlayer()->playSound(EnemyDestorySound);//播放敌人死亡的声音
        m_game->awardGold(100);
        getRemoved();
    }
}
void EnemyBase::getRemoved()
{
    if (m_attackedTowers.empty())
        return;

    foreach (TowerBase *attacker, m_attackedTowers)
        attacker->targetKilled();

    m_game->removeEnemy(this);
}
bool EnemyBase::check(Target *target)
{
    vector<EnemyBase*> Enemylist=m_game->getEnemies();
    for(vector<EnemyBase*>::iterator iter=Enemylist.begin();iter!=Enemylist.end();iter++)
    {
        if(*iter==target)
            return true;//该敌人仍存在
    }
    return false;
}
void EnemyBase::getAttacked(TowerBase *attacker)
{
    m_attackedTowers.push_back(attacker);
}
void EnemyBase::getLostSight(TowerBase *attacker)
{
    for(vector<TowerBase*>::iterator iter=m_attackedTowers.begin();iter!=m_attackedTowers.end();iter++)
    {
        if(*iter==attacker)
        {
            m_attackedTowers.erase(iter);
            break;
        }
    }
}

QPoint EnemyBase::pos() const
{
    return m_pos;
}
void EnemyBase::doActivate()
{
    m_active = true;
}
void EnemyBase::checkAttactingTower()
{return;}
void EnemyBase::setAttackingTargetNull()
{}


const QSize Enemy::ms_fixedSize(85, 76);

Enemy::Enemy(WayPoint *startWayPoint, MyGame *game, const QPixmap &sprite)
    : EnemyBase(startWayPoint,game,1.0,sprite)
{
}
Enemy::~Enemy()
{
    m_attackedTowers.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}
void Enemy::setAttackingTargetNull()
{}
void Enemy::checkAttactingTower()
{return;}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//中心位点的左上方
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);
    //绘制冰冻状态
    if(m_iced)
    {
        QPoint IceBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, ms_fixedSize.height() / 2-5);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        QRect IcedRect(IceBarPoint, QSize(Health_Bar_Width, 2));
        painter->drawRect(IcedRect);
    }

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

const QSize shootingEnemy::ms_fixedSize(101, 76);

shootingEnemy::shootingEnemy(WayPoint *startWayPoint, MyGame *game, const QPixmap &sprite,int maxhp)
    : EnemyBase(startWayPoint,game,2.0,sprite,maxhp)
    , m_fireRate(500)
    , attackingTarget(NULL)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}
void shootingEnemy::checkAttactingTower()
{
    if(m_attackedTowers.empty())
    {
        return;
    }
    else
    {
        attackingTarget=m_attackedTowers.front();
        m_fireRateTimer->start(m_fireRate);
        return;
    }
}
void shootingEnemy::shootWeapon()
{
    if(m_game->checkTower(attackingTarget))
    {
    Bullet1 *bullet1 = new Bullet1(m_pos, attackingTarget->getpos(), 0, attackingTarget, m_game);
    bullet1->move();
    m_game->audioPlayer()->playSound(EnemyShootSound);
    m_game->addBullet(bullet1);
    }
    else m_fireRateTimer->stop();
}
void shootingEnemy::setAttackingTargetNull()
{
    attackingTarget=NULL;
}
void shootingEnemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//中心位点的左上方
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    //绘制冰冻状态
    if(m_iced)
    {
        QPoint IceBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, ms_fixedSize.height() / 2-5);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        QRect IcedRect(IceBarPoint, QSize(Health_Bar_Width, 2));
        painter->drawRect(IcedRect);
    }


    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

destroyingEnemy::destroyingEnemy(WayPoint *startWayPoint,MyGame *game, const QPixmap &sprite)
    :EnemyBase(startWayPoint,game,1.5,sprite,80)
    ,attackingrange(80)
{}
const QSize destroyingEnemy::ms_fixedSize(101, 76);
void destroyingEnemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;

    painter->save();

    painter->setPen(Qt::green);
    // 绘制攻击范围
    painter->drawEllipse(m_pos, attackingrange, attackingrange);

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 2+5);//中心位点的左上方
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    //绘制冰冻状态
    if(m_iced)
    {
        QPoint IceBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, ms_fixedSize.height() / 2-5);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::blue);
        QRect IcedRect(IceBarPoint, QSize(Health_Bar_Width, 2));
        painter->drawRect(IcedRect);
    }

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}
void destroyingEnemy::destroy()
{
    QList<TowerBase*> Towerlist=m_game->getTowers();
    foreach(TowerBase* Tower,Towerlist)
    {
        if(collisionWithCircle(m_pos, attackingrange, Tower->getpos(), 1))
        {
            m_game->removeTower(Tower);
           // break;
        }
    }
}
void destroyingEnemy::getDamage(int damage,bool ice)
{
    m_currentHp -= damage;
    if(ice)
    {
        m_walkingSpeed=1.0*2/3;
        m_iced=true;
    }
    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        m_game->awardGold(200);
        destroy();
        getRemoved();
    }
}
shootingEnemy::~shootingEnemy()
{
    m_attackedTowers.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}
