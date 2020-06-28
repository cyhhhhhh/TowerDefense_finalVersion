#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>

class QMediaPlayer;

enum SoundType
{
    TowerPlaceSound,		// ����ʱ������
    LifeLoseSound,			// ���ط�Ѫʱ������
    LaserShootSound,		// ���ڵ�ʱ������
    EnemyDestorySound,		// ��������ʱ������
    TowerDestroySound,       // �����ݻٵ�����
    EnemyShootSound        // ���˷����ڵ�������
};

class AudioPlayer : public QObject
{
public:
    explicit AudioPlayer(QObject *parent = 0);

    void startBGM();
    void playSound(SoundType soundType);

private:
    QMediaPlayer *m_backgroundMusic; // ֻ�������ű�������
};

#endif // MUSIC_H
