#include "convexhull.h"
bool cmpXandY(Point p1,Point p2){
    float xandy1 = p1.x+p1.y;
    float xandy2 = p2.x+p2.y;
    return xandy1<xandy2;
}
bool cmpXsubY(Point p1,Point p2){
    float xandy1 = p1.x-p1.y;
    float xandy2 = p2.x-p2.y;
    return xandy1<xandy2;
}
bool ConvexHull::IsLeftPoint(Point pt, Line line)
{
    Point v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
    Point v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
    float tmp =Cross2D(v1,v2);
    if(tmp>=0) // 0 on the right side 0 on the extended line
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool ConvexHull::IsRightPoint(Point pt, Line line)
{
    Point v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
    Point v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
    float tmp =Cross2D(v1,v2);
    if(tmp>0) // 0 in the right side
    {
        return true;
    }else
    {
        return false;
    }
}

float ConvexHull::Cross2D(Point a, Point b)
{
    return a.x*b.y-b.x*a.y;
}

QList<Point> ConvexHull::getDispts() const
{
    return dispts;
}

QList<Triangle> ConvexHull::getTins() const
{
    return tins;
}

QList<Triangle> ConvexHull::getDelaunay(QList<Triangle> hulltins, QList<Point> pts)
{
    int onLine = 0;
    for(int i = 0; i<pts.size(); i++)
    {
        QList<Triangle> delTin; //Preservation essential triangle
        for(int j =0; j< hulltins.size(); j++)
        {

//            Circle tinCircle = Circle::genTriCircle(hulltins[j]);
            //            if(tinCircle.isInCircle(vec3(pts[i])))  //In front of the circumscribed circle, a search-like triangular circumscribed circle
            //            {
            //                delTin.push_back(hulltins[j]);
            //            }

            if(hulltins[j].isInTriangle(pts[i]) == true)
            {
                delTin.push_back(hulltins[j]);
//                qDebug()<<"del index"<< j;
            }
            if(hulltins[j].isOnTriangle(pts[i]) == true){
                delTin.push_back(hulltins[j]);
                onLine++;
            }
        }
        QList<Line> borderLines;//Find adjacent edges of discrete points
        QList<Triangle> newTri;
        if(delTin.size() == 1)
        {
            Line l1 = delTin[0].l1;
            Line l2 = delTin[0].l2;
            Line l3 = delTin[0].l3;
            borderLines.push_back(l1);
            borderLines.push_back(l2);
            borderLines.push_back(l3);

            for(int j = 0; j< borderLines.size();j++)
            {
                newTri.push_back(Triangle(borderLines[j].p1,borderLines[j].p2,pts[i]));
            }
            hulltins.removeOne(delTin[0]);
        }else if(delTin.size() == 2)
        {

             Line l[3];
             l[0] = delTin[0].l1;
             l[1] = delTin[0].l2;
             l[2] = delTin[0].l3;
             int index = 0;
             for( int m =0;m< 3; m++)
                 if(delTin[1].containsLine(l[m]) ==0)
                 {
                     borderLines.push_back(l[m]);
                 }else
                 {
                    index =  delTin[1].containsLine(l[m])-1;
                 }
            for( int m =0;m< 3; m++)
            {
                if(m!=index)
                    borderLines.push_back(delTin[1].l[m]);
            }


            for(int j = 0; j< borderLines.size();j++)
            {
                newTri.push_back(Triangle(borderLines[j].p1,borderLines[j].p2,pts[i]));
            }
            hulltins.removeOne(delTin[0]);
            hulltins.removeOne(delTin[1]);
        }

        delTin.clear();
        for( int s = 0; s< newTri.size(); s++){
            for(int j = 0; j< 3 ; j++){
                Line line = newTri[s].l[j];
                for( int m = 0; m< hulltins.size() ; m++)
                {
                    {
                        if(hulltins[m].containsLine(line) )
                        {
                            Circle tinCircle = Circle::genTriCircle(hulltins[m]);
                            if(tinCircle.isInCircle(vec3(pts[i])))
                            {
                                delTin.push_back(newTri[s]);

                                int x = hulltins[m].containsLine(line)-1;
                                //                                    borderLines.removeOne(line);
                                for( int k = 0; k<3;k ++)
                                {
                                    if(x!=k){
                                        //                                            borderLines.push_back(hulltins[m].l[k]);
                                        newTri.push_back(Triangle(hulltins[m].l[k].p1,hulltins[m].l[k].p2,pts[i]));
                                    }
                                }
                                hulltins.removeAt(m);
//                                qDebug()<<"in tricircle index"<< m;
                            }
//                            qDebug()<<"edge tri index"<< m;

                        }
                    }
                }
            }

        }

        for( int m =0; m< newTri.size() ; m++)
        {
            hulltins.push_back(newTri[m]);
        }
        for( int m = 0; m< delTin.size() ; m++)
        {
            hulltins.removeOne(delTin[m]);
        }

//                else
//                {
//                    QList<Line> borderLines;//
//                    for(int j =0; j< delTin.size(); j++)
//                    {
//                        Line l1 = delTin[j].l1;
//                        Line l2 = delTin[j].l2;
//                        Line l3 = delTin[j].l3;
//                        bool repeatLine[3] = {false};
//                        for( int m = 0; m< delTin.size() ; m++)
//                        {
//                            if(j!=m)
//                            {
//                                if(delTin[m].containsLine(l1) )
//                                {
//                                    repeatLine[0]  = true;
//                                }
//                                if(delTin[m].containsLine(l2) )
//                                {
//                                    repeatLine[1]  = true;

//                                }
//                                if(delTin[m].containsLine(l3) )
//                                {
//                                    repeatLine[2]  = true;

//                                }
//                            }
//                        }
//                        if(repeatLine[0] == false&&repeatLine[1] == false&&repeatLine[2] == false)
//                        {
//                            if(delTin[j].isInTriangle(pts[i]) == false ){
//                                delTin.removeAt(j);
//                                j--;
//                                continue;
//                            }
//                        }
//                        if(repeatLine[0] == false)
//                            borderLines.push_back(l1);
//                        if(repeatLine[1] == false)
//                            borderLines.push_back(l2);
//                        if(repeatLine[2] == false)
//                            borderLines.push_back(l3);
//                    }

//                            }
//                            QList<Triangle> test;
//                    for(int j = 0; j< borderLines.size();j++)
//                    {
//                        hulltins.push_back(Triangle(borderLines[j].p1,borderLines[j].p2,pts[i]));
//                    }

//                             qDebug()<<"lines  num "<<borderLines.size();
//                    qDebug()<<"del max "<<delTin.size();
//                                     tins = delTin;
//                    for( int m = 0; m< delTin.size() ; m++)
//                    {
//                        hulltins.removeOne(delTin[m]);
//                    }
//                }

    }
//    qDebug()<<"online num"<<onLine;
    tins = hulltins;
    return tins;
}

ConvexHull::ConvexHull()
{

}

void ConvexHull::generateHull(QVector<Point> pts)
{
    QList<Point> othpts;
    hullpts.clear();
    for(int i =0; i< pts.size(); i++)
    {
        othpts.push_back(pts[i]);
    }
    //：min(x-y)、min(x+y)、max(x-y)、max(x+y)
    Sort::quickSort(pts,0,pts.size()-1,cmpXsubY);

    hullpts.push_back(pts[0]);
    hullpts.push_back(pts[pts.size()-1]);
    othpts.removeOne(pts[0]);
    othpts.removeOne(pts[pts.size()-1]);
    Sort::quickSort(pts,0,pts.size()-1,cmpXandY);
    if(!hullpts.contains(pts[0]))
    {
        hullpts.insert(1,pts[0]);
    }
    if(!hullpts.contains(pts[pts.size()-1]))
    {
        hullpts.push_back(pts[pts.size()-1]);
    }

    othpts.removeOne(pts[0]);
    othpts.removeOne(pts[pts.size()-1]);
    int i = 0;
    bool over = true;
    int maxdisIndex = 0;


    while(i<hullpts.size())  //
    {
        Line cline;//
        if(i == hullpts.size() -1 ) //
        {
            cline.init(hullpts[i],hullpts[0]);
        }else
        {
            cline.init(hullpts[i],hullpts[i+1]);
        }
        float maxdis = 0;

        for( int m = 0; m< othpts.size(); m++)
        {
            if(IsRightPoint(othpts[m],cline))  //
            {
                float dist = Gemetry::PointToLine(othpts[m],cline);

                if(dist > maxdis)
                {
                    maxdis = dist;
                    maxdisIndex = m;
                    over =false; //
                }
            }
        }
        if(over == true) //
        {
            i++;
        }else{//
            hullpts.insert(i+1,othpts[maxdisIndex]);
            othpts.removeAt(maxdisIndex);
            over = true;
        }

    }
    dispts = othpts;

}

QList<Triangle> ConvexHull::DivideHell(QList<Point> pts)
{
    QList<Point> hpts;
    for(int i = 0; i< pts.size() ; i++)
        hpts.push_back(pts[i]);
    tins.clear();
    int index = 0;
    while(hpts.size() >2)
    {
        int tag = index;
        float minangle = 180;
        float maxangle = 0;

        for(int i = index; i< hpts.size() ; i++)
        {
            float tri_angle = 180.0;

            if(i == 0){
                tri_angle = Gemetry::angle3D(hpts.last(),hpts[i],hpts[i+1]);

            }
            else if(i == hpts.size()-1){
                tri_angle = Gemetry::angle3D(hpts[i-1],hpts[i],hpts[0]);
                //                tri.init(hpts[i-1],hpts[i],hpts[0]);
            }
            else{
                tri_angle = Gemetry::angle3D(hpts[i-1],hpts[i],hpts[i+1]);
                //                tri.init(hpts[i-1],hpts[i],hpts[i+1]);
            }
            if(tri_angle < minangle)
            {
                tag = i;
                minangle = tri_angle;
            }
            //            tag = i;

        }
        int tagb = tag-1;
        int tage = tag+1;
        if(tag == 0)
            tagb =hpts.size()-1;
        if(tag == hpts.size()-1)
            tage = 0;
        tins.push_back(Triangle(hpts[tagb],hpts[tag],hpts[tage]));

        hpts.removeAt(tag);
        //         index = (tag+1)%hpts.size();


    }

    //    for(int j =0; j< tins.size(); j++)
    //    {
    //        Line l1 = tins[j].l1;
    //        Line l2 = tins[j].l2;
    //        Line l3 = tins[j].l3;
    //        bool repeatLine[3] = {false};
    //        Point p1 = tins[j].p3;
    //        Point p2 = tins[j].p1;
    //        Point p3 = tins[j].p2;
    //        for( int m = 0; m< tins.size() ; m++)
    //        {
    //            if(j!=m)
    //            {
    //                if(tins[m].containsLine(l1) )
    //                {
    //                    repeatLine[0]  = true;
    //                    Circle tricircle = Circle::genTriCircle(tins[m]);
    //                    if(tricircle.isInCircle(vec3(p1)))

    //                }
    //                if(tins[m].containsLine(l2) )
    //                {
    //                    repeatLine[1]  = true;

    //                }
    //                if(tins[m].containsLine(l3) )
    //                {
    //                    repeatLine[2]  = true;

    //                }
    //            }
    //        }
    //        if(repeatLine[0] == false)
    //            borderLines.push_back(l1);
    //        if(repeatLine[1] == false)
    //            borderLines.push_back(l2);
    //        if(repeatLine[2] == false)
    //            borderLines.push_back(l3);
    //}

    return tins;
}
