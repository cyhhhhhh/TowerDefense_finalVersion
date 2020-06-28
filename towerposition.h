#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap(":/image/spot.png"));

    void setHasTower(bool hasTower = true);//����������m_hasTower��Ϊtrue
    bool hasTower() const;//��λ���Ƿ�����
    void setLoseTower(bool hasTower=false);
    QPoint centerPos() const;//centerPos��ʾ������ʵ�ʰ��ŵ�λ�ã����롰�رꡱλ���е�СС��ƫ��
    bool containPoint(const QPoint &pos) const;

    void draw(QPainter *painter) const;

private:
    bool		m_hasTower;
    QPoint		m_pos;
    QPixmap		m_sprite;

    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
