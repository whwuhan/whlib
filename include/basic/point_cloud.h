#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <basic/point3d.h>
#include <basic/cube.h>
#include <set>
#include <glm/glm.hpp>
namespace wh{
    namespace basic{
        const int POINT3D_SIZE = 3;
        //点云
        typedef struct PointCloud{
            // C中的struct不允许定义static变量，但是C++的可以，一般直接编译会出现warning
            // static enum pointType{POINT, SPHERE};
            //点云数据
            Eigen::MatrixXd points;
            //点云大小
            unsigned int size;

            //适配Coolender
            unsigned int VAO;//点状点云的VAO
            unsigned int VBO;//点状点云的VBO
            bool show;//是否显示
            glm::vec4 color;//颜色
            glm::mat4 model;//model矩阵
            float scale;//缩放大小
            float transX;//X轴的位移
            float transY;//Y轴的位移
            float transZ;//Z轴的位移
            float rotateX;//绕X轴旋转
            float rotateY;//绕Y轴旋转
            float rotateZ;//绕Z轴旋转
            float pointSize;//点的大小
            bool changePointSize;//是否改变了点云的pointSize
            
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
            //获取点云体素在cube体素中的位置 vector里面是0表示当前cube体素不在点云体素中
            //1表示在点云的体素中
            std::vector<int> getVoxelIndex(wh::basic::Cube& boundingBox,double leafSize);

            //获取Boundingbox
            wh::basic::Cube getBoundingBox();

            //获取C++原生数据
            // float* getGLData();

        } POINT_CLOUD;
        //注意这里要声明友元函数，结构体里面不是声明这个函数，而是说明这个是友元函数
        std::ostream& operator<<(std::ostream& ost, const PointCloud pointCloud);

        //体素化点云
        typedef struct VoxelPointCloud{

        } VOXEL_POINT_CLOUD;
    }
}