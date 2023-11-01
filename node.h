#ifndef NODE_H
#define NODE_H


class Node
{
public:
    Node();
    int weight;                 //权值
    int parent, lch, rch;       //双亲结点、左孩子结点、右孩子结点
};

#endif // NODE_H
