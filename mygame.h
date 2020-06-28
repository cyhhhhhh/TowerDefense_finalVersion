#ifndef MYGAME_H
#define MYGAME_H
#include <QMainWindow>
#include <QList>
#include <qpainter.h>
#include <vector>
#include <QTimer>
#include "towerposition.h"
#include "tower.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "music.h"
QT_BEGIN_NAMESPACE
using namespace std;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class EnemyBase;
class BulletBase;
class TowerBase;
class AudioPlayer;
class TowerPosition;
class MyGame : public QMainWindow
{
    Q_OBJECT
public:
    MyGame(QWidget *parent = nullptr);
    void getHpDamage(int damage = 1);
    void removeEnemy(EnemyBase *enemy);
    void addBullet(BulletBase *bullet);
    void removedBullet(BulletBase *bullet);
    void removeTower(TowerBase*tower);
    void awardGold(int money);
    void updateTower(TowerBase*tower);
    bool checkTower(TowerBase* tower);
    bool checkEnemy(EnemyBase* enemy);
    AudioPlayer* audioPlayer() const;
    vector<EnemyBase*> getEnemies() const;
    QList<TowerBase*> getTowers() const;
private slots:
    void updateMap();
protected:
    void paintEvent(QPaintEvent *);
    bool canbuytower();
    bool canupdatetower();
    void mousePressEvent(QMouseEvent *);
private:

    vector <TowerPosition> Towerpositions;
    QList  <TowerBase*> Towers;
    vector <WayPoint*> Waypoints;
    vector <EnemyBase*> Enemies;
    vector <BulletBase*> Bullets;

    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    bool gameWin;
    bool gameLost;
    int  wave;
    int  playerHp;
    int  gold;
    AudioPlayer * m_audioPlayer;
    bool loadwave();
    void loadTowerPositions();//载入“地标”位置
    void loadWayPoints();//载入航点(“碰撞点”）
    void doGameOver();

signals:

};

#endif // MYGAME_H
