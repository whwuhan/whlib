#ifndef SKELETON_H
#define SKELETON_H
#include <iostream>
#include <Eigen/Dense>
#include <basic/abstract_data_type/curve.h>
#include <basic/point3d.h>
namespace wh{
    namespace basic{   
        typedef struct Skeleton{
            Eigen::MatrixXd points;//骨架点
            Eigen::MatrixXi edges;//骨架边
            
            Skeleton();
            Skeleton(Eigen::MatrixXd& points,Eigen::MatrixXi& edges);

            Eigen::MatrixXi get_adj_mat();//获取邻接矩阵
            
            template<typename T>
            std::vector< wh::basic::adt::Curve<T> > get_branches();
        } SKELETON;
    }
}
#endif