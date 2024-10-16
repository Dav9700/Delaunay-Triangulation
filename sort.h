#ifndef SORT_H
#define SORT_H
#include <QVector>
#include <QDebug>
#include <QVector3D>
#include <QImage>
#define PAI 3.1415
class Point{
public:
    float x,y,z;
    Point(){}
    Point(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    void init(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    bool operator ==(const Point &p)
    {
        return fabs(x-p.x)<= 1e-6&&fabs(y-p.y)<= 1e-6&&fabs(z-p.z)<= 1e-6;
    }
    Point operator +(const Point& p1)
    {
        return Point(p1.x+x,p1.y+y,p1.z+z);
    }
    Point operator -(const Point& p1)
    {
        return Point(x-p1.x,y-p1.y,z-p1.z);
    }

    void print()
    {
        qDebug()<<"("<<x<<","<<y<<","<<z<<")";
    }

};
class vec3{  //Vector
public:
    float x,y,z;
    vec3(){}
    vec3(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    vec3(Point a){ x = a.x; y = a.y;z = a.z;}
    vec3(Point a,Point b){ x = b.x-a.x; y = b.y-a.y; z = b.z-a.z; }  //a Oriented b-oriented quantity
    void init(float x1,float y1,float z1=0){x = x1;y = y1;z = z1;}
    bool operator ==(const vec3 &p)
    {
        return fabs(x-p.x)<= 1e-6&&fabs(y-p.y)<= 1e-6&&fabs(z-p.z)<= 1e-6;
    }
    vec3 operator +(const vec3& p1)
    {
        return vec3(p1.x+x,p1.y+y,p1.z+z);
    }
    vec3 operator -(const vec3& p1)
    {
        return vec3(x-p1.x,y-p1.y,z-p1.z);
    }


};
struct Line{
    Point p1,p2; //Directed line segment，p1 Oriented p2
    Line(){}
    Line(Point p1,Point p2){this->p1 = p1;this->p2 = p2; }
    void init(Point p1,Point p2){this->p1 = p1;this->p2 = p2; }
    bool operator ==(const Line &l)
    {
        return p1==l.p1&& p2 == l.p2;
    }
};


class Gemetry
{
public:
    Gemetry() {}
    static vec3 Cross(vec3 a, vec3 b)
    {
        float x = a.y*b.z - a.z*b.y;
        float y = a.z*b.x - a.x*b.z;
        float z = a.x*b.y - a.y*b.x;
        return  vec3(x,y,z);
    }
    //Calculated distance
    static float PointToPoint(Point pos1, Point pos2)
    {
        float sum = 0;
        sum+= pow(pos2.x-pos1.x,2)+pow(pos2.y-pos1.y,2)+pow(pos2.z-pos1.z,2);
        return sqrt(sum);
    }
    static float PointToPoint(vec3 pos1, vec3 pos2)
    {
        float sum = 0;
        sum+= pow(pos2.x-pos1.x,2)+pow(pos2.y-pos1.y,2)+pow(pos2.z-pos1.z,2);
        return sqrt(sum);
    }
    //
    static float Norm(vec3 v){
        float sum = 0;
        sum+= pow(v.x,2)+pow(v.y,2)+pow(v.z,2);
        return sqrt(sum);
    }
    //
    static float PointToLine(Point p,Line l)
    {
        vec3 ab(l.p1,l.p2);
        vec3 ac(l.p1,p);
        return Norm(Cross(ab,ac))/PointToPoint(l.p1,l.p2); 
    }

    //    static float angle(float x1,float y1, float x2,float y2){
    //        float n = (x1*x2+y1*y2);
    //        float m = sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2);
    //        return acos(n/m)*180/PAI;
    //    }
    static float angle3D(Point a,Point b,Point c){
        vec3 v1(a-b);
        vec3 v2(c-b);
        float x1 = v1.x;
        float y1 = v1.y;
        float z1 = v1.z;
        float x2 = v2.x;
        float y2 = v2.y;
        float z2 = v2.z;
        float n = (x1*x2+y1*y2 + z1*z2);
        float m = sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2 +z2*z2);
        return acos(n/m)*180/PAI;
    }
    //
    static float angle3D(QVector3D n1,QVector3D n2)
    {
        float x1 = n1.x();
        float y1 = n1.y();
        float z1 = n1.z();
        float x2 = n2.x();
        float y2 = n2.y();
        float z2 = n2.z();
        float n = (x1*x2+y1*y2 + z1*z2);
        float m = sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2 +z2*z2);
        return acos(n/m)*180/PAI;
    }
    static float Cross2D(vec3 a, vec3 b)
    {
        return a.x*b.y-b.x*a.y;
    }
    static bool IsRightPoint(vec3 pt, Line line)
    {
        vec3 v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
        vec3 v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
        float tmp =Cross2D(v1,v2);
        if(tmp>0) //
        {
            return true;
        }else
        {
            return false;
        }
    }
    static bool IsOnLine(vec3 pt, Line line)
    {
        vec3 v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
        vec3 v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
        float tmp =Cross2D(v1,v2);
        float minx,miny;
        float maxx,maxy;
        if(line.p1.x < line.p2.x){
            minx = line.p1.x;
            maxx = line.p2.x;
        }
        else{
            minx = line.p2.x;
            maxx = line.p1.x;
        }

        if(line.p1.y < line.p2.y){
            miny= line.p1.y;
            maxy = line.p2.y;
        }
        else{
            miny = line.p2.y;
            maxy = line.p1.y;
        }
        if(fabs(tmp)<=1e-5 && pt.x>minx&&pt.x < maxx&& pt.y>miny && pt.y < maxy) //Greater than 0 on the right
        {
            return true;
        }else
        {
            return false;
        }
    }
};
struct Triangle{
    Point p1,p2,p3;//
    Point p[3];
    Line l1,l2,l3;//
    Line l[3];//
    Triangle(){}
    Triangle(Point a,Point b,Point c){ init(a,b,c);}
    bool isInTriangle(Point a)
    {
        bool r1 = Gemetry::IsRightPoint(a,l1);
        bool r2 = Gemetry::IsRightPoint(a,l2);
        bool r3 = Gemetry::IsRightPoint(a,l3);
        if(r1 ==r2&& r2 == r3)
            return true;
        return false;
    }
    int isOnTriangle(Point a)
    {
        bool r1 = Gemetry::IsOnLine(a,l1);
        bool r2 = Gemetry::IsOnLine(a,l2);
        bool r3 = Gemetry::IsOnLine(a,l3);
        if(r1)
            return 1;
        if(r2)
            return 2;
        if(r3)
            return 3;
        return 0;
    }

    void init(Point a,Point b,Point c){
        p1 =a ;
        p2 =b;
        p3 = c;
        p[0] = a;
        p[1] = b;
        p[2] = c;
        l1 = Line(a,b);
        l2 = Line(b,c);
        l3 = Line(c,a);
        l[0] = l1;
        l[1] = l2;
        l[2] = l3;
    }
    int  containsLine(Line l)
    {
        if((l.p1 == p1 && l.p2 == p2) || (l.p1 == p2 && l.p2 == p1) )
            return 1;
        if((l.p1 == p2 && l.p2 == p3) || (l.p1 == p3 && l.p2 == p2) )
            return 2;
        if((l.p1 == p1 && l.p2 == p3) || (l.p1 == p3 && l.p2 == p1) )
            return 3;
        return 0;
    }
    bool operator ==(const Triangle& tri)
    {
        if(p1 == tri.p1 && p2 == tri.p2&& p3 == tri.p3)
            return true;
        return false;
    }
};
struct Circle{
    double radius;
    vec3 center;
    Circle(){}
    Circle(vec3 cent,double r){ center = cent; radius = r;}
    static Circle genTriCircle(Triangle tri){
        Point p1 = tri.p1;
        Point p2 = tri.p2;
        Point p3 = tri.p3;
        double  x1,x2,x3,y1,y2,y3;
        x1  =  p1.x;
        x2  =  p2.x;
        x3  =  p3.x;
        y1  =  p1.y;
        y2  =  p2.y;
        y3  =  p3.y;
        //
        double a=sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
        double b=sqrt( (x1-x3)*(x1-x3)+(y1-y3)*(y1-y3) );
        double c=sqrt( (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3) );
        double p=(a+b+c)/2;
        double S=sqrt( p*(p-a)*(p-b)*(p-c) );

        //
        double t1=x1*x1+y1*y1;
        double t2=x2*x2+y2*y2;
        double t3=x3*x3+y3*y3;
        double temp=x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
        double x=(t2*y3+t1*y2+t3*y1-t2*y1-t3*y2-t1*y3)/temp/2;
        double y=(t3*x2+t2*x1+t1*x3-t1*x2-t2*x3-t3*x1)/temp/2;
        double r=a*b*c/(4*S);
        vec3 cent(x,y,0);
        return Circle(cent,r);

    }
    bool isInCircle(vec3 v)
    {
        double dist = Gemetry::PointToPoint(center,v);
        return dist<radius;
    }
};



class Sort
{
public:
    Sort() {};

    static void quickSort(QVector<float>&a,int left,int right,bool(*cmp)(float ,float))
    {
        if(left<right){
            int i=left;
            int j=right;
            float  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp,a[j]))
                    j--;
                if(j>i)
                    a[i++]=a[j];
                while(i<j&&cmp(a[i],temp))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;
            quickSort(a,left,i-1,cmp);
            quickSort(a,i+1,right,cmp);
        }
    }
    static void quickSort(QVector<Point>&a,int left,int right,bool(*cmp)(Point ,Point))
    {
        if(left<right){
            int i=left;
            int j=right;
            Point  temp=a[left];
            if(left>=right)
                return;
            while(i<j)
            {
                while(i<j&& cmp(temp,a[j]))
                    j--;
                if(j>i)
                    a[i++]=a[j];
                while(i<j&&cmp(a[i],temp))
                    i++;
                if(i<j)
                    a[j--]=a[i];
            }
            a[i]=temp;
            quickSort(a,left,i-1,cmp);
            quickSort(a,i+1,right,cmp);
        }
    }
};
class Image{
public:
    static int getIndex(int i, int width)
    {
        if(i<0)
            i = 0;
        if(i>=width)
            i = width-1;
        return i;
    }
    static QImage TransToEdge(const QImage &source)
    {
        int w = source.width();
        int h = source.height();
        int Gmax =140;
        QImage Edge(w,h,QImage::Format_RGB32);

        for( int i = 0; i< h; i++){
        //Convolution operation
            for(int j = 0; j < w; j++){
                double Gx =  (-1)* QColor(source.pixel(getIndex(j-1,w),getIndex(i-1,h))).red()
                            +(-2)*QColor(source.pixel(getIndex(j,w),getIndex(i-1,h))).red()
                            +(-1)*QColor(source.pixel(getIndex(j+1,w),getIndex(i-1,h))).red()
                            +QColor(source.pixel(getIndex(j-1,w),getIndex(i+1,h))).red()
                            +2*QColor(source.pixel(getIndex(j,w),getIndex(i+1,h))).red()
                            +QColor(source.pixel(getIndex(j+1,w),getIndex(i+1,h))).red();

                double Gy =  QColor(source.pixel(getIndex(j-1,w),getIndex(i-1,h))).red()
                        +(2)*QColor(source.pixel(getIndex(j-1,w),getIndex(i,h))).red()
                        +(1)*QColor(source.pixel(getIndex(j-1,w),getIndex(i+1,h))).red()
                        +(-1)*QColor(source.pixel(getIndex(j+1,w),getIndex(i-1,h))).red()
                        +(-2)*QColor(source.pixel(getIndex(j+1,w),getIndex(i,h))).red()
                        +(-1)*QColor(source.pixel(getIndex(j+1,w),getIndex(i+1,h))).red();

               double G = sqrt(Gx*Gx+Gy*Gy);

                QRgb pixel;
                if(G>Gmax)
                  pixel = qRgb(255,255,255);
                else
                  pixel = qRgb(0,0,0);
                Edge.setPixel(j,i,pixel);
            }
        }
        return Edge;
    }
};

#endif // SORT_H
