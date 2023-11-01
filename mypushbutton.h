#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QPaintEvent>
#include <QPixmap>
#include <QDebug>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton();
    MyPushButton(QString address, QWidget* widget);  //参数为其父窗口
    QString addresspic;                              //按钮图片地址
    QPixmap pix;                                     //渲染按钮图片调色板
};

#endif // MYPUSHBUTTON_H
