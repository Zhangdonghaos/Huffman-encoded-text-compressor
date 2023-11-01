#include "mypushbutton.h"

MyPushButton::MyPushButton()
{

}

MyPushButton::MyPushButton(QString address, QWidget* widget)
{
    this->setParent(widget);
    this->setWindowFlag(Qt::FramelessWindowHint); //设置无边框化
    addresspic = address;
    bool ret = pix.load(addresspic);
    if (!ret)
    {
        qDebug() << addresspic << "图片加载失败！";
    }
    this->setStyleSheet("QPushButton{border:0px}"); //设置不规则图片的样式表
    this->setIcon(pix);
    this->setIconSize(QSize(pix.size()));
    this->setFixedSize(pix.size());

}
