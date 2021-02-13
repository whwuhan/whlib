#ifndef CUBE_H
#define CUBE_H
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <basic/point3d.h>
namespace wh
{
    namespace basic
    {
        typedef struct Cube
        {
            Eigen::MatrixXd vertices;    //正方形顶点
            Eigen::RowVector3d position; //立方体的位置(中心位置)
            double sideLen;             //边长

            //长方体边长
            double x;
            double y;
            double z;

            //构造函数
            Cube();                                                          //默认构造函数
            Cube(Eigen::MatrixXd vertices);                                  //8个顶点描述cube
            Cube(Eigen::RowVector3d position, double sideLen);              //使用中心位置和边长描述cube
            Cube(Eigen::RowVector3d position, double x, double y, double z); //使用中心位置和边长描述cuboid

            //运算符重载
            bool operator<(const Cube &cube) const;

            void showInf();                             //显示cube信息
            void showInf() const;                       //显示cube信息
            void positionSideLenToVertices();        //位置边长表示转化成顶点表示（正方体）
            void positionSideLenToVerticesCuboid(); //位置边长表示转化成顶点表示（长方体）
            Eigen::RowVector3d getCenter();             //获取中心
            double getMaxSideLen();                   //获取最长边长

            //细分
            std::vector<wh::basic::Cube> subdivision(int amount);       //参数：总细分的个数
            std::vector<wh::basic::Cube> subdivision(double leaf_size); //参数：细分的立方体边长

        } CUBE;
    } // namespace basic
} // namespace wh
#endif