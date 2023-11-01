#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "node.h"
#include <QByteArray>
#include <map>
#include <iomanip>
#include <QString>
#include <QDebug>

using namespace std;

class HuffmanTree : public Node
{
public:
    HuffmanTree();
    HuffmanTree(QByteArray &arr);
    HuffmanTree(map<char, int> &tmp_map);
    ~HuffmanTree();

    void Select(Node* HFT, int m, int& s1, int& s2);                   //获得权值最小的结点
    Node* GetNode();                                                   //获得哈夫曼树的起始节点
    void Trans_To_Code(HuffmanTree* hft, QString*&hft_code, int num);  //将字符转换为二进制编码，并存入字典
    int GetSize();                                                     //获得字典的size


    QByteArray array;
    map<char, int>hft_map;     //字典
    Node* node;
};

#endif // HUFFMANTREE_H
