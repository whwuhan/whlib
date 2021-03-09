#pragma once
#include <Eigen/Dense>
#include <basic/edge.h>
#include <basic/face.h>
#include <set>
#include <map>
#include <glm/glm.hpp>
namespace wh
{
    namespace basic
    {
        typedef struct PolygonMesh
        {
            Eigen::MatrixXd vertices; //多边型顶点
            Eigen::MatrixXi indices;  //多边形面片索引

            //适配Coolender
            unsigned int VAO;//mesh的VAO
            unsigned int VBO;//mesh的VBO
            unsigned int EBO;//mesh的EBO
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






            //std::set<wh::basic::Edge> edges;//边集合
            //构造函数
            PolygonMesh();
            PolygonMesh(Eigen::MatrixXd &vertices, Eigen::MatrixXi &indices);

            //mesh 细分
            void triMeshSubdivision(); //三角面片细分
            void loopSubdivision();     //loop细分 TODO

            //=======其他功能
            //生成边集
            std::set<wh::basic::Edge> creatUnorderedEdges(); //无序边
            std::set<wh::basic::Edge> creatOrderedEdges();   //有序边

            //找到边的相邻面片
            std::map<wh::basic::Edge, std::set<wh::basic::Face> > findEdgeNearFaces(std::set<wh::basic::Edge> &edges);
        } POLYGON_MESH;
    } // namespace basic
} // namespace wh