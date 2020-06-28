#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>

class QMediaPlayer;

enum SoundType
{
    TowerPlaceSound,		// 放塔时的声音
    LifeLoseSound,			// 基地费血时的声音
    LaserShootSound,		// 发炮弹时的声音
    EnemyDestorySound,		// 敌人升天时的声音
    TowerDestroySound,       // 塔被摧毁的声音
    EnemyShootSound        // 敌人发射炮弹的声音
};

class AudioPlayer : public QObject
{
public:
    explicit AudioPlayer(QObject *parent = 0);

    void startBGM();
    void playSound(SoundType soundType);

private:
    QMediaPlayer *m_backgroundMusic; // 只用来播放背景音乐
};

#endif // MUSIC_H
