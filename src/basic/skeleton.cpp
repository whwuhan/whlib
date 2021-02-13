#include <basic/skeleton.h>
#include <iostream>
#include <set>
#include <utils/io/io_obj.h>
using namespace std;
using namespace wh::basic;
using namespace wh::utils::io;
using namespace Eigen;
bool isAllVisited(vector<int> &flag);                                     //是否所有点都被访问
int getNextDegree1Index(vector<int> &flag, vector<int> &pointsDegree); //获取下一个度为一的，没有被访问的点位置
wh::basic::Skeleton::Skeleton() : points(), edges()
{
}

wh::basic::Skeleton::Skeleton(Eigen::MatrixXd &points, Eigen::MatrixXi &edges) : points(points), edges(edges)
{
}

Eigen::MatrixXi wh::basic::Skeleton::getAdjMat()
{
    int size = points.rows();
    MatrixXi adjMat = MatrixXi::Zero(size, size);
    for (int row = 0; row < edges.rows(); row++)
    {
        adjMat(edges(row, 0) - 1, edges(row, 1) - 1) = 1;
        adjMat(edges(row, 1) - 1, edges(row, 0) - 1) = 1;
    }
    return adjMat;
}

//模板方法实现
//显示特化要放到实现文件中，不能放到头文件中
//获取分支数组
template <>
std::vector<wh::basic::Curve<wh::basic::Point3d> > wh::basic::Skeleton::getBranches()
{
    MatrixXi adjMat = getAdjMat(); //获取邻接矩阵

    //获取顶点的度
    vector<int> pointsDegree(points.rows(), 0);
    for (int row = 0; row < adjMat.rows(); row++)
    {
        for (int col = 0; col < adjMat.cols(); col++)
        {
            if (adjMat(row, col) == 1)
            {
                pointsDegree[row]++;
            }
        }
    }
    // for(int i=0;i<pointsDegree.size();i++){
    //     cout << "pointsDegree" << i << ": " << pointsDegree[i] <<endl;
    // }

    //获取分支数组
    vector<Curve<Point3d>> res;
    vector<int> flag(points.rows(), 0); //当前顶点是否被访问

    int row = 0;
    while (getNextDegree1Index(flag, pointsDegree) != -1)
    {
        int nextOneDegreeIndex = getNextDegree1Index(flag, pointsDegree);
        Curve<Point3d> curve;
        //获取下一个度为一的，没有被访问的点位置
        if (-1 != nextOneDegreeIndex)
        {
            Point3d newPoint(points.row(nextOneDegreeIndex));
            curve.addPointBack(newPoint);
            row = nextOneDegreeIndex;
            flag[nextOneDegreeIndex] = 1;
        }
        //获取一条分支
        while (true)
        {
            int last_row = row;
            for (int col = 0; col < adjMat.cols(); col++)
            {
                if (adjMat(row, col) == 1 && flag[col] == 0)
                {
                    Point3d newPoint(points.row(col));
                    curve.addPointBack(newPoint);
                    flag[col] = 1;
                    row = col;
                    break;
                }
            }
            // cout << "last_row " << last_row <<endl;
            // cout << "row " << row <<endl;
            if (last_row == row)
            {
                //curve.show();
                res.push_back(curve);
                // saveCurvesObj("curve.obj",&res);
                break;
            }
        }
    }
    // saveCurvesObj("curve.obj",&res);
    return res;
}

//获取相邻骨架点的平均间隔距离
double wh::basic::Skeleton::getAveInterval()
{
    int edgeSize = edges.rows();
    double dis_sum = getTolLen();
    return dis_sum / edgeSize;
}

double wh::basic::Skeleton::getTolLen()
{
    int edgeSize = edges.rows();
    double dis_sum = 0.0;
    for (int i = 0; i < edgeSize; i++)
    {
        dis_sum += (points.row(edges(i, 0) - 1) - points.row(edges(i, 1) - 1)).norm();
        //cout << "dis_sum:" << dis_sum << endl;
    }
    return dis_sum;
}

//是否所有点都被访问
bool isAllVisited(vector<int> &flag)
{
    for (int i : flag)
    {
        if (i == 0)
        {
            return false;
        }
    }
    return true;
}

//获取下一个度为一的，没有被访问的点位置
int getNextDegree1Index(vector<int> &flag, vector<int> &pointsDegree)
{
    for (int i = 0; i < flag.size(); i++)
    {
        if (flag[i] == 0 && pointsDegree[i] == 1)
        {
            return i;
        }
    }
    return -1;
}
