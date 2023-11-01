#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(600, 400);
    this->setWindowTitle("基于哈夫曼表的文本压缩器");

    btn_add = new MyPushButton(":/res/add.png", this);            //按钮
    btn_to = new MyPushButton(":/res/to.png", this);
    btn_delete = new MyPushButton(":/res/delete.png", this);
    btn_compress = new MyPushButton(":/res/compress.png", this);
    btn_test = new QPushButton(this);
    btn_decompress = new QPushButton(this);

    label_add = new QLabel(this);                                //按钮文本标识
    label_to = new QLabel(this);
    label_delete = new QLabel(this);

    label_outcome = new QLabel(this);                            //文本框功能标识
    label_holded = new QLabel(this);

    edit_holded_show = new QTextEdit(this);                      //文本框
    edit_outcome_show = new QTextEdit(this);

    grid_layout = new QGridLayout;                               //布局
    v_layout = new QVBoxLayout;

    grid_layout->addWidget(btn_add, 0, 0, 1, 1);
    grid_layout->addWidget(btn_to, 0, 1, 1, 1);
    grid_layout->addWidget(btn_delete, 0, 2, 1, 1);
    grid_layout->addWidget(btn_compress, 0, 3, 2, 1);
    grid_layout->addWidget(btn_test, 1, 3, 2, 1);
    grid_layout->addWidget(btn_decompress, 0, 4, 6, 1);
    grid_layout->addWidget(label_add, 1, 0, 1, 1);
    grid_layout->addWidget(label_to, 1, 1, 1, 1);
    grid_layout->addWidget(label_delete, 1, 2, 1, 1);


    v_layout->addLayout(grid_layout);
    v_layout->addWidget(label_holded);
    v_layout->addWidget(edit_holded_show);
    v_layout->addWidget(label_outcome);
    v_layout->addWidget(edit_outcome_show);

    this->setLayout(v_layout);
    label_add->setText("添加文件");
    label_to->setText("压缩到");
    label_delete->setText("删除");
    label_outcome->setText("进程如下：");
    label_holded->setText("已添加的文件名如下：");
    btn_test->setText("测试哈夫曼树");
    btn_decompress->setText("解压缩");
    btn_decompress->setFixedSize(100, 40);

    Connects();
}

void Widget::Connects()
{
    connect(btn_add, &QPushButton::clicked, [=](){
        path_from = QFileDialog::getOpenFileName(this, "打开文件", "C:/Users/DELL/Desktop");
        QFile file(path_from);
        file.open(QIODevice::ReadWrite);

        edit_holded_show->setText(file.fileName());

        array = file.readAll();
        have_address_from = 1;
    });

    connect(btn_to, &QPushButton::clicked, [=](){
        dia = new QDialog;
        btn_dia = new QPushButton(dia);
        edit_dia = new QTextEdit(dia);

        dia->setWindowTitle("请输入地址：");
        dia->setFixedSize(300, 70);

        edit_dia->resize(dia->width(), dia->height() - 43);
        btn_dia->move(115, 37);
        btn_dia->setText("确定");

        dia->show();

        connect(btn_dia, &QPushButton::clicked, [=](){
            path_to = edit_dia->toPlainText();
            dia->close();
            have_address_to = 1;
        });
    });

    connect(btn_delete, &QPushButton::clicked, [=](){
        edit_holded_show->clear();
        path_from = "";
        have_address_from = 0;
    });

    connect(btn_compress, &QPushButton::clicked, [=](){
        if (!have_address_to)
            {
            QMessageBox::critical(this, "错误", "未输入压缩地址");
        }
        else if (!have_address_from)
            {
            QMessageBox::critical(this, "错误", "未输入原文件地址");
        }
        else
            {
            //这里开始转码
            edit_outcome_show->setText("开始转码");
            hft = new HuffmanTree(array);
            int num = hft->GetSize();
            hft_code = new QString[num + 1];  //转码后的哈夫曼编码存到 hft_code 内
            hft->Trans_To_Code(hft, hft_code, num);
            edit_outcome_show->append("转码完成，开始存入文件");

            //这里开始把转换后的二进制码存入file文件
            Store(path_to);
            edit_outcome_show->append("存入文件完成\n");

            //这里开始计算压缩效率
            GetEfficiency();
        }


    });

    connect(btn_test, &QPushButton::clicked, [=](){
        HuffmanTree* tmp_hft = new HuffmanTree(array);
        Test(tmp_hft);
    });

    connect(btn_decompress, &QPushButton::clicked, [=](){
        dia = new QDialog; //封装为成员函数
        btn_dia = new QPushButton(dia);
        edit_dia = new QTextEdit(dia);

        dia->setWindowTitle("请输入解压缩文件地址：");
        dia->setFixedSize(300, 70);

        edit_dia->resize(dia->width(), dia->height() - 43);
        btn_dia->move(115, 37);
        btn_dia->setText("确定");

        dia->show();              //弹出对话框，输入解压缩文件地址

        connect(btn_dia, &QPushButton::clicked, [=](){
            path_de_compress = edit_dia->toPlainText();
            dia->close();
            DeCompress();            //开始解压缩
        });

    });
}

void Widget::Test(HuffmanTree* tmp_hft)
{

    QFont font;
    font.setWordSpacing(50);
    edit_outcome_show->setFont(font);

    edit_outcome_show->insertPlainText("i ");
    edit_outcome_show->insertPlainText("weight ");
    edit_outcome_show->insertPlainText("parent ");
    edit_outcome_show->insertPlainText("lchild ");
    edit_outcome_show->insertPlainText("rchild ");

    for (int i = 1; i <= tmp_hft->hft_map.size() * 2 - 1; i++)
    {
        edit_outcome_show->append(QString::number(i) + "    " + QString::number(tmp_hft->node[i].weight) + "    " + QString::number(tmp_hft->node[i].parent) + "    " + QString::number(tmp_hft->node[i].lch) + "    " + QString::number(tmp_hft->node[i].rch));
    }
}

void Widget::Store(QString path_store)
{
//存入哈夫曼编码，便于解压缩
    //1.存入所有出现过的字符, 以 “#” 为结束符
    //2.存入这些字符出现频数
    //3.以 “@”为结束符
    QString s, code;
    map<char, int>::iterator it = hft->hft_map.begin();
    while(it != hft->hft_map.end())                         //1
    {
        s += it->first;
        s += " ";
        ++it;
    }
    for (int i = 1; i <= hft->GetSize(); ++i)               //2
    {
        code += QString::number(hft->node[i].weight);
        code += " ";
    }

    QFile file(path_store);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        qDebug() << "压缩后文件打开失败， 地址为：" << path_store;
    file.write(s.toLatin1() + '\n' + '#' + '\n');
    file.write(code.toLatin1() + '\n' + '@' + '\n');        //3

//存储原文本的哈夫曼代码
    //1.得到0-1串
    //2.将0-1串8位并做1位得到二进制代码，不足8位的用0补齐
    //3.将二进制代码存入二进制文件
    QString zero_one = "";
    for (int i = 0; i < array.size(); i++)                  //1
    {
        it = hft->hft_map.begin();
        int j;
        for (j = 1; it->first != array[i]; ++j, ++it);
        zero_one += hft_code[j];
    }
    string zero_one_s = zero_one.toStdString();
    QString s_zero_one = "";
    while(zero_one_s.size() >= 8)                           //2.1
    {
        bitset<8>bits(zero_one_s, 0, 8); //将0-8位置的元素并做1个字节的二进制数（8位）
        zero_one_s.erase(0, 8);          //删除0-8位置的元素
        char a = bits.to_ulong();        //得到一个字节的二进制数代表的字符
        s_zero_one += a;
    }
    file.write(s_zero_one.toLatin1());   //3

    if (!zero_one_s.empty())                                //2.2
    {
        int loc = zero_one_s.size();
        bitset<8>bits(zero_one_s, 0, loc);
        char a = bits.to_ulong();
        QString s = "";
        s += a;
        file.write(s.toLatin1());   //3
        file.write(QByteArray::number(loc)); //将最后剩余的二进制代码长度写入文件结尾，解码时按照位数读出
    }
    else file.write(QByteArray::number(0));
    file.close();
}
void Widget::GetEfficiency()
{
    QFile file_from(path_from);
    if (!file_from.open(QIODevice::ReadOnly))
        qDebug() << "压缩文件打开失败，地址为：" << path_from;
    QFileInfo info(file_from);
    double size_from = info.size();

    QFile file_to(path_to);
    if (!file_to.open(QIODevice::ReadOnly))
        qDebug() << "压缩文件打开失败，地址为：" << path_to;
    info.setFile(file_to);
    double size_to = info.size();

    edit_outcome_show->append("压缩文件大小为（byte）： " + QString::number(size_from));
    edit_outcome_show->append("压缩后文件大小为（byte）：" + QString::number(size_to));
    edit_outcome_show->append("压缩效率为：" + QString::number((1 - size_to / size_from) * 100) + '%');
}

void Widget::DeCompress()
{
    QFile file(path_de_compress);
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "解压缩文件打开失败，文件地址为：" << path_de_compress;
    QByteArray array_de = file.readAll();
    QByteArray s_de = "";
    if (array_de[0] == ' ')
        s_de = " ";
    int i;
    for (i = 0; i < array_de.size(); ++i)
    {
        if (array_de[i] == '#') break;
        if (array_de[i] != ' ')
            s_de += array_de[i];
    }
    s_de.resize(s_de.size() - 1);
    //接收字符出现的频次
    QString sum;
    QVector<int>vec;
    for (i = i + 2; i < array_de.size(); ++i)
    {
        if (array_de[i] == '@') break;
        if (array_de[i] != ' ')
        {
            sum += array_de[i];
        }
        else
        {
            vec.push_back(sum.toInt());
            sum = "";
        }
    }

    //存入map内
    map<char, int>tmp_map;
    for (int j = 0; j < vec.size(); ++j)
    {
        tmp_map.insert({s_de[j], vec[j]});
    }

    //解决末尾位数不为8的情况
      //1.找到loc值与该字符
      //2.将字符转换为二进制字符串
      //3.取前loc位保存
      //4.删除该字符与计数字符（倒数第二个和最后一个字符）
    int loc = array_de.at(array_de.size() - 1) - '0'; //末尾的位数
    QString s_tmp;
    if (loc != 0)
    {
        int val = (int)array_de[array_de.size() - 2];
        s_tmp = QString::number(val, 2);
        array_de.erase(&array_de[array_de.size() - 2], array_de.end());
    }
    else
    {
        array_de.erase(&array_de[array_de.size() - 1], array_de.end());
    }

    //将剩余字符串转换为0-1串
      //1.将剩余字符串读出,并将字符转换成十进制数
      //2.将十进制数转换成二进制数，存入待处理的0-1串内
    QVector<unsigned int>vec1;
    QString s_remain = "";
    for (int j = i + 2; j < array_de.size(); ++j)
    {
        unsigned char c = array_de[j];
        vec1.push_back(c);
    }
    for (int k = 0; k < vec1.size(); ++k)
    {
        QString num = QString::number(vec1[k], 2);
        if (num.size() < 8)
            for (int k = num.size(); k < 8; k++)
                num.push_front('0');
        s_remain += num;
    }
    s_remain += s_tmp;

    //创建哈夫曼树
    HuffmanTree* hft_de = new HuffmanTree(tmp_map);
    //qDebug() << hft_de->GetSize();

    //获得0-1串内蕴含的字符值
    QString s_final;
    Node* tmpnode = &hft_de->node[hft_de->GetSize() * 2 - 1];  //根节点
    for (int k = 0; k < s_remain.size(); ++k)
    {
        int m; //存储前一个结点的左、右孩子
        if (s_remain[k] == '0')
        {
            m = tmpnode->lch;
            tmpnode = &hft_de->node[m];
        }
        else if (s_remain[k] == '1')
        {
            m = tmpnode->rch;
            tmpnode = &hft_de->node[m];
        }
        if (tmpnode->lch == 0 && tmpnode->rch == 0)  //如果为叶子结点
        {
            map<char, int>::iterator it = hft_de->hft_map.begin();
            advance(it, (m - 1));
            s_final += it->first;
            tmpnode = &hft_de->node[hft_de->GetSize() * 2 - 1];
        }
    }


    //将得到的字符值传入新文件
    QFile file_de("C:/Users/DELL/Desktop/3.txt");
    if (!file_de.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "解压缩后文件无法打开，地址为：" << "C:/Users/DELL/Desktop/3.txt";
    }
    file_de.write(s_final.toLatin1());
}

Widget::~Widget()
{
    delete grid_layout; delete v_layout ;delete btn_add; delete btn_to;
    delete btn_delete; delete btn_compress; delete btn_dia; delete btn_test;
    delete btn_decompress; delete label_add; delete label_to; delete label_delete;
    delete label_outcome; delete label_holded; delete edit_holded_show;
    delete edit_outcome_show; delete edit_dia; delete ui; delete dia;
    delete hft;delete []hft_code;
}

