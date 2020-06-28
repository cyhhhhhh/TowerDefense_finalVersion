#include "button.h"
#include <QPixmap>
button::button(QString Pic):QPushButton(0)
{
    QPixmap pixmap(Pic);
    this->setFixedSize(pixmap.width(),pixmap.height());
    this->setStyleSheet("QPushButton{border:Opx;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(),pixmap.height()));

}
