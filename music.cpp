#include "music.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>

static const QString s_curDir = QDir::currentPath() + "/";

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
    , m_backgroundMusic(NULL)
{
    // ����һֱ���ŵı�������
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "music/bgm.mp3");
    if (QFile::exists(backgroundMusicUrl.toLocalFile()))
    {
        m_backgroundMusic = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backgroundMusicUrl);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // ���ñ�������ѭ������
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic->setPlaylist(backgroundMusicList);
    }
}

void AudioPlayer::startBGM()
{
    if (m_backgroundMusic)
        m_backgroundMusic->play();
}

void AudioPlayer::playSound(SoundType soundType)
{
    static const QUrl mediasUrls[] =
    {
        QUrl::fromLocalFile(s_curDir + "music/tower_place.wav"),
        QUrl::fromLocalFile(s_curDir + "music/life_lose.wav"),
        QUrl::fromLocalFile(s_curDir + "music/laser_shoot.mp3"),
        QUrl::fromLocalFile(s_curDir + "music/enemy_destroy.mp3"),
        QUrl::fromLocalFile(s_curDir + "music/tower_destroy.mp3"),
        QUrl::fromLocalFile(s_curDir + "music/shoot_sound.Mp3")
    };
    static QMediaPlayer player;

    if (QFile::exists(mediasUrls[soundType].toLocalFile()))
    {
        player.setMedia(mediasUrls[soundType]);
        player.play();
    }
}
