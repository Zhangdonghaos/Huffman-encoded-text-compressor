#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mypushbutton.h"
#include <QTextEdit>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDataStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include "huffmantree.h"
#include <bitset>
#include <QMessageBox>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void Connects();
    void Test(HuffmanTree* tmp_hft);  //测试哈夫曼树
    void Store(QString path_store);   //压缩、存入文件
    void DeCompress();                //解压缩
    void GetEfficiency();             //计算压缩效率

private:
    Ui::Widget *ui;

    QGridLayout* grid_layout;         //栅格布局
    QVBoxLayout* v_layout;            //垂直布局

    MyPushButton* btn_add;
    MyPushButton* btn_to;
    MyPushButton* btn_delete;         //删除按钮
    MyPushButton* btn_compress;       //压缩按钮
    QPushButton* btn_dia;             //对话框内确定按钮
    QPushButton* btn_test;            //测试哈夫曼表按钮
    QPushButton* btn_decompress;      //解压缩按钮

    QLabel* label_add;                //按钮功能标识
    QLabel* label_to;
    QLabel* label_delete;
    QLabel* label_outcome;            //文本框功能标识
    QLabel* label_holded;             //文本框功能标识

    QTextEdit* edit_holded_show;
    QTextEdit* edit_outcome_show;
    QTextEdit* edit_dia;

    QString path_from;                //待压缩文件地址
    QString path_to;                  //压缩后文件地址
    QString path_de_compress;         //待解压缩文件地址

    QDialog* dia;

    QByteArray array;                 //存储文本内字符串
    HuffmanTree* hft;                 //哈夫曼树
    QString* hft_code;                //基于哈夫曼树生成的二进制编码表

    bool have_address_from = 0;       //判断用户是否输入目标文件地址
    bool have_address_to = 0;         //判断用户是否输入压缩地址


};
#endif // WIDGET_H
