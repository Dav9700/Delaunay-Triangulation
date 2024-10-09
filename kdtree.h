#ifndef KDTREE_H
#define KDTREE_H

#include <QVector3D>
#include <QRectF>
#include <QVector>
#include <QDebug>
#include <QtAlgorithms>
#include <cmath>
#include <time.h>
struct Range{   //Spatial data structure
    float Left,Right,Top,Bottom,Front,Back;
    float left(){return Left;}
    float right(){return Right;}
    float top(){return Top;}
    float bottom(){return Bottom;}
    float front(){return Front;}
    float back(){return Back;}
    float height(){return Bottom-Top;}
    float width(){return Right-Left;}
    float length(){return Back-Front;}
    Range(float left,float top,float front,float right,float bottom,float back)
    {
         Left=left;
         Right=right;
         Top=top;
         Bottom=bottom;
         Front=front;
         Back=back;
    }
    Range(){}
};
struct Data{
    QVector3D value;
    int index;
    Data(){}
    Data(QVector3D v,int i)
    {value = v;index = i; }
    bool operator ==(const Data& d)
    {
        if(d.index==index&& d.value == value)
            return true;
        else
            return false;
    }
};


struct KdNode{
//    QVector3D data;
    Data data;
    Range range;
    int split; //Determine which plane to split 0：x ，1：y ，2：z；
    KdNode *left;//Left subtree
    KdNode *right;//Right subtree
    KdNode *father;//Father subtree
    KdNode()
    {
        split = -1;
        left = NULL;
        right = NULL;
        father = NULL;
    }
};

class KdTree
{
public:
    KdTree();
    void init(Range range,QVector<QVector3D>data_set); //Initialization function
    void init(Range range,QVector<Data>data_set); //Initialization function
    void sortTree();
    Data search(KdNode* node , QVector3D pos, bool *mask); //Find the nearest neighbor
    QVector<Data> search(KdNode *node,float radius,QVector3D pos);//Find range point
    KdNode *header() const;
    float dist(QVector3D p1,QVector3D p2);
private:
    KdNode *m_header;
    QVector<Data> m_data; //Initial data set
    QVector<Data> m_datax;
    QVector<Data> m_datay;
    QVector<Data> m_dataz;
    KdNode* BuildTree(Range range, QVector<Data> data_set, KdNode *parent);//Build kd tree
    void KDTraverse(KdNode* node);  //Output function
    float variance(QVector<Data> &a, int dimen); //variance

    bool cmp(const QVector3D a,const QVector3D b,int demin)
    {
        if(demin == 0)
            return a.x()<b.x();
        if(demin == 1)
            return a.y()<b.y();
        if(demin == 2)
            return a.z()<b.z();
        return false;
    }
    void quickSort(QVector<Data>&a,int left,int right,int demin)
    {
        if(left<right){
            int i=left;
            int j=right;
            Data  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp.value,a[j].value,demin))
                    j--;
                if(j>i)
                    a[i++]=a[j];//a[i]Has been assigned to temp, so directly assign a[j] to a[i], after the assignment, a[j], there is a space
                while(i<j&&cmp(a[i].value,temp.value,demin))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;//Insert the benchmark, at this time i and j are equalR[low..pivotpos-1].keys≤R[pivotpos].key≤R[pivotpos+1..high].keys
            quickSort(a,left,i-1,demin);/*Recursive left*/
            quickSort(a,i+1,right,demin);/*Recursive right*/
        }
    }


};

#endif // KDTREE_H
