#ifndef SKELETON_H
#define SKELETON_H
#include <iostream>
#include <Eigen/Dense>
#include <basic/curve.h>
#include <basic/point3d.h>
namespace wh
{
    namespace basic
    {
        typedef struct Skeleton
        {
            Eigen::MatrixXd points; //骨架点
            Eigen::MatrixXi edges;  //骨架边

            Skeleton();
            Skeleton(Eigen::MatrixXd &points, Eigen::MatrixXi &edges);

            Eigen::MatrixXi getAdjMat(); //获取邻接矩阵

            double getTolLen();      //获取骨架线总的长度
            double getAveInterval(); //获取相邻骨架点的平均间隔距离

            template <typename T>
            std::vector<wh::basic::Curve<T> > getBranches();
        } SKELETON;
    } // namespace basic
} // namespace wh
#endif