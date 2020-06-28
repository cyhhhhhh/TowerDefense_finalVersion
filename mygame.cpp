#include "mygame.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"towerposition.h"
#include<QMouseEvent>
#include"music.h"
#include<iostream>
using namespace std;
MyGame::MyGame(QWidget *parent)
    :QMainWindow(parent)
  , gameWin(false)
  , gameLost(false)
  , wave(0)
  , playerHp(5)
  , gold(1000)
{
    this->setFixedSize(800,600);
    loadTowerPositions();
    loadWayPoints();
    loadwave();
    QTimer *timer = new QTimer(this);
    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM();//播放背景音乐
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
}


void MyGame::removeEnemy(EnemyBase *enemy)
{

    for(vector<EnemyBase*>::iterator iter=Enemies.begin();iter!=Enemies.end();iter++)
    {
        if(*iter==enemy)
        {
            Enemies.erase(iter);
            break;
        }
    }//利用迭代器删除Enemies中的指定对象
    delete enemy;//敌人被消灭，需要移除
    if (Enemies.empty())
    {
        wave++;
        if (!loadwave())
        {
            gameWin = true;
            // 游戏胜利转到游戏胜利场景
            // 这里暂时以打印处理
        }
    }
}

vector<EnemyBase *> MyGame::getEnemies() const
{
    return Enemies;
}

void MyGame::updateMap()
{
    foreach (EnemyBase *enemy, Enemies)
    {
        enemy->checkAttactingTower();
        enemy->move();
    }
    foreach (TowerBase *tower, Towers)
        tower->checkEnemyInRange();
    update();
}

void MyGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (gameLost|| gameWin)
    {
        if(gameWin)
        {
            painter.drawPixmap(0,0,QPixmap(":/image/victory.jpg"));
        }
        else painter.drawPixmap(0,0,QPixmap(":/image/lose.jpg"));
        return;
    }

    painter.drawPixmap(0, 0, QPixmap(":/image/bg.png"));
    painter.drawPixmap(656,409,QPixmap(":/image/base.png"));
    for(int i=0;i<(int)Towerpositions.size();i++)
    {
        Towerpositions[i].draw(&painter);
    }
    for(int i=0;i<(int)Towers.size();i++)
    {
        Towers[i]->draw(&painter);
    }
    for(int i=0;i<(int)Enemies.size();i++)
    {
        Enemies[i]->draw(&painter);
    }
    for(int i=0;i<(int)Bullets.size();i++)
    {
        Bullets[i]->draw(&painter);
    }
    drawWave(&painter);
    drawHP(&painter);
    drawPlayerGold(&painter);
}

bool MyGame::canbuytower()
{
    if (gold >=300)
        return true;
    else return false;
}
bool MyGame::canupdatetower()
{
    if(gold>=300)
        return true;
    else return false;
}

void MyGame::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    QList<TowerBase*> Towerlist=getTowers();
    for(int i=0;i<(int)Towerlist.size();i++)
    {
        if(canbuytower()&&Towerlist[i]->containPoint(pressPos)
                        &&Towerlist[i]->checkupdate())
        {
            Towerlist[i]->update();
            update();
            break;
        }
    }
    for(int i=0;i<(int)Towerpositions.size();i++)
    {
        if (canbuytower() && Towerpositions[i].containPoint(pressPos)
                          && !Towerpositions[i].hasTower())
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            gold -= 300;//减去购买所需费用
            Towerpositions[i].setHasTower();//该位置有塔了
            Tower* tower = new Tower(Towerpositions[i].centerPos(), this);
            Towers.push_back(tower);
            update();
            break;
        }
    }
}
bool MyGame::checkTower(TowerBase *tower)
{
    QList<TowerBase*> Towerlist=getTowers();
    for(QList<TowerBase*>::iterator iter=Towerlist.begin();iter!=Towerlist.end();iter++)
    {
        if(*iter==tower)
        {
            return true;
            break;
        }
    }
    return false;
}
bool MyGame::checkEnemy(EnemyBase *enemy)
{
    vector<EnemyBase*> Enemylist=getEnemies();
    for(vector<EnemyBase*>::iterator iter=Enemylist.begin();iter!=Enemylist.end();iter++)
    {
        if(*iter==enemy)
        {
            return true;
            break;
        }
    }
    return false;
}
AudioPlayer *MyGame::audioPlayer() const
{
    return m_audioPlayer;
}

bool MyGame::loadwave()
{
    if (wave >= 50)
        return false;
    WayPoint *startWayPoint = Waypoints.front();
    vector<int> interval1 = {1000,2000,3000,4000,5000,6000};
    vector<int> interval2 = {1000,3000,5000,6000,7000,8000};
    vector<int> interval3 = {1000,2000,4000,7000,8000,10000};
    vector<int> interval4 = {1000,2000,5000,6000,8000,9000};
    vector<int> interval;
    int n=qrand()%5;
    switch(n)
    {
    case 0:interval=interval1;break;
    case 1:interval=interval1;break;
    case 2:interval=interval2;break;
    case 3:interval=interval3;break;
    case 4:interval=interval4;break;
    }
    for (int i = 0; i < 6; ++i)
    {
        if(wave<5)
        {
             Enemy *enemy = new Enemy(startWayPoint, this);
             Enemies.push_back(enemy);
             QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
        }
        else if(wave<10)
        {
            if(i==4||i==1)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
        }
        else if(wave<20)
        {
            if(i==4||i==1)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
        }
        else if(wave<30)
        {
            if(i==0)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else if(i==2)
            {
                destroyingEnemy  *enemy = new destroyingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
        }
        else if(wave<40)
        {
            if(i==0)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else if(i==2||i==4)
            {
                destroyingEnemy  *enemy = new destroyingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
        }
        else if(wave<50)
        {
            if(i==1)
            {
                shootingEnemy *enemy = new shootingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            if(i==3||i==5||i==2)
            {
                destroyingEnemy  *enemy = new destroyingEnemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
            else
            {
                Enemy *enemy = new Enemy(startWayPoint, this);
                Enemies.push_back(enemy);
                QTimer::singleShot(interval[i], enemy, SLOT(doActivate()));
            }
        }
    }
    return true;
}


void MyGame::loadTowerPositions()
{
    QPoint pos[]=
    {
        QPoint(166,166),
        QPoint(294,166),
        QPoint(437,166),
        QPoint(557,166),

        QPoint(200,280),
        QPoint(329,280),
        QPoint(465,280),
        QPoint(590,280),

        QPoint(180,405),
        QPoint(307,405),
        QPoint(443,405),
        QPoint(569,405)
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0;i<len;i++)
    {
        Towerpositions.push_back(pos[i]);
    }
}

void MyGame::loadWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(16,243));
    Waypoints.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(675,243));
    Waypoints.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(675,355));
    Waypoints.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(130,355));
    Waypoints.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(130,485));
    Waypoints.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(677,485));
    Waypoints.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);
}

void MyGame::getHpDamage(int damage)
{
    m_audioPlayer->playSound(LifeLoseSound);//播放玩家HP减少的声音
    playerHp -= damage;
    if (playerHp <= 0)
        doGameOver();
}

void MyGame::doGameOver()
{
    if (!gameLost)
    {
        gameLost = true;
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

void MyGame::addBullet(BulletBase *bullet)
{

    Bullets.push_back(bullet);
}


void MyGame::removedBullet(BulletBase *bullet)
{
    Q_ASSERT(bullet);
    for(vector<BulletBase*>::iterator iter=Bullets.begin();iter!=Bullets.end();iter++)
    {
        if(*iter==bullet)
        {
            Bullets.erase(iter);
            break;
        }
    }//利用迭代器删除Bullets中的指定元素
    delete bullet;
}


void MyGame::awardGold(int money)
{
    gold += money;//奖励金币
    update();
}

void MyGame::removeTower(TowerBase *tower)
{
    Towers.removeOne(tower);
    int i;
    for(i=0;i<(int)Towerpositions.size();i++)
    {
        if(tower->getpos()==Towerpositions[i].centerPos())
            break;
    }
    Towerpositions[i].setLoseTower();
    delete tower;
    audioPlayer()->playSound(TowerDestroySound);
}
void MyGame::updateTower(TowerBase *tower)
{
    Towers.removeOne(tower);
    UpdatedTower* Tower=new UpdatedTower(tower->getpos(),this);
    delete tower;
    Towers.push_back(Tower);
    gold-=Tower->knowcost();
}



void MyGame::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(681, 120, 90, 28), QString("Wave: %1").arg(wave + 1));
}

void MyGame::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(31, 120, 92, 28), QString("Health: %1").arg(playerHp));
}

void MyGame::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(356, 120, 98, 28), QString("GOLD: %1").arg(gold));
}

QList<TowerBase*> MyGame::getTowers()const
{
    return Towers;
}
