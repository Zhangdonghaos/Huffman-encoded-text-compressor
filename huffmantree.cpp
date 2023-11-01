#include "huffmantree.h"
#include <algorithm>

HuffmanTree::HuffmanTree()
{

}

HuffmanTree::HuffmanTree(QByteArray &arr) : array(arr)
{
    int num = array.size();
    map<char, int>::iterator it;
    for (int i = 0; i < num; i++)        //获得 所有叶子结点 及其 权值
    {
        it = hft_map.find(array[i]);
        if (it != hft_map.end())
        {
            it->second++;
        }
        else
        {
            hft_map.insert({array[i], 1});
        }
    }
    num = 2 * hft_map.size() - 1;        //叶子结点个数

    node = new Node[num + 1];
    for (int i = 1; i <= num; i++)       //将1-num所有结点初始化
    {
        node[i].weight = 0;
        node[i].parent = 0;
        node[i].lch = 0;
        node[i].rch = 0;
    }

    it = hft_map.begin();
    int i = 1;
    for (i = 1; i <= (int)hft_map.size(); ++i, ++it)  //将所得权值传入数组
    {
        node[i].weight = it->second;
    }

    //下面开始创建哈夫曼树
    while(i <= num)
    {
        int s1 = 0, s2 = 0;                      //为i-1的原因：此时i为hft_map.size()+1
        Select(node, i - 1, s1, s2);             //从1到i-1结点中寻找权值最小的两个结点，并将结点位置返回至s1,s2
        node[s1].parent = i; node[s2].parent = i;// i为新结点
        node[i].lch = s1;    node[i].rch = s2;
        node[i++].weight = node[s1].weight + node[s2].weight;
    }
}

HuffmanTree::HuffmanTree(map<char, int> &tmp_map)
{
    hft_map = tmp_map;

    int num = hft_map.size() * 2 - 1;
    node = new Node[num + 1];
    for (int i = 1; i <= num; i++)       //将1-num所有结点初始化
    {
        node[i].weight = 0;
        node[i].parent = 0;
        node[i].lch = 0;
        node[i].rch = 0;
    }
    map<char, int>::iterator it = hft_map.begin();
    int i;
    for (i = 1; i <= (int)hft_map.size(); ++i, ++it)
    {
        node[i].weight = it->second;
    }
    //下面开始创建哈夫曼树
    while(i <= num)
    {
        int s1 = 0, s2 = 0;
        Select(node, i - 1, s1, s2);
        node[s1].parent = i; node[s2].parent = i;
        node[i].lch = s1;    node[i].rch = s2;
        node[i++].weight = node[s1].weight + node[s2].weight;
    }
}

void HuffmanTree::Select(Node* HFT, int m, int& s1, int& s2)
{
    //若频数大于十万怎么办？用unsign long long
    int tmp = 100000;
    for(int i = 1; i <= m; i++)    //寻找权值最小的结点
    {
        if (HFT[i].parent == 0 && tmp > HFT[i].weight)
        {
                tmp = HFT[i].weight;
                s1 = i;
        }
    }

    tmp = 100000;
    for (int i = 1; i <= m; i++)  //寻找权值第二小的结点
    {
        if (HFT[i].parent == 0 && tmp > HFT[i].weight && i != s1)
        {
                tmp = HFT[i].weight;
                s2 = i;
        }
    }

}

void HuffmanTree::Trans_To_Code(HuffmanTree* hft,QString*&hft_code, int num)
{
    int sign_parent, sign_node;
    for (int i = 1; i <= num; i++)
    {
        sign_parent = hft->node[i].parent;    //当前遍历结点的双亲结点*
        sign_node = i;                        //当前遍历结点
        while(sign_parent)
        {
                if (hft->node[sign_parent].lch == sign_node) //当前结点是双亲结点的左子树  编码为0
                    hft_code[i] += '0';
                else hft_code[i] += '1';                     //反之，编码为1
                sign_node = sign_parent;                     //当前结点向上移
                sign_parent = hft->node[sign_parent].parent; //双亲结点向上移，直到遍历至根节点
        }
        reverse(hft_code[i].begin(), hft_code[i].end()); //得到编码后需逆置
    }
}

Node* HuffmanTree::GetNode()
{
    return node;
}

int HuffmanTree::GetSize()
{
    return hft_map.size();
}

HuffmanTree::~HuffmanTree()
{
    delete []node;
}
