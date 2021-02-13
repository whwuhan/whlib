#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <basic/point3d.h>
#include <basic/cube.h>
#include <set>
namespace wh{
    namespace basic{
        const int POINT3D_SIZE = 3;
        //点云
        typedef struct PointCloud{
            //点云数据
            Eigen::MatrixXd points;
            //点云大小
            unsigned int size;
            
            //构造函数
            PointCloud();
            PointCloud(const unsigned int size, const int pointSize = POINT3D_SIZE);
            PointCloud(const Eigen::MatrixXd& points);

            //重载运算符
            PointCloud operator+(const PointCloud& pointCloud);
            PointCloud operator-(const PointCloud& pointCloud);
            PointCloud operator*(const Eigen::MatrixXd transformMatrix);
            Point3d operator[](const unsigned int index);
            friend std::ostream& operator<<(std::ostream& ost, const PointCloud pointCloud);

            //改变维度，注意resize()会改变矩阵内部数据
            void resize(const unsigned int rows, const unsigned int cols);

            //如果维度比原有的维度小，那么数据不改变，相当于裁剪矩阵，如果扩张了维度，数据会是随机值
            void conservativeResize (const unsigned int rows, const unsigned int cols);

            //获取点云几何中心
            Eigen::RowVector3d getGeometricCenter();

            //将点云放回原点
            void getCenteredPointCloud();

            //归一化点云
            wh::basic::Cube getNormalizedPointCloud();

            //点云转化为vector存储
            std::vector<Point3d> pointsToVector();

            //点云体素化
            std::set<wh::basic::Cube> voxelization(wh::basic::Cube& boundingBox,double leafSize);

            //获取Boundingbox
            wh::basic::Cube getBoundingBox();

        } POINT_CLOUD;
        //注意这里要声明友元函数，结构体里面不是声明这个函数，而是说明这个是友元函数
        std::ostream& operator<<(std::ostream& ost, const PointCloud pointCloud);

        //体素化点云
        typedef struct VoxelPointCloud{

        } VOXEL_POINT_CLOUD;
    }
}
#endif