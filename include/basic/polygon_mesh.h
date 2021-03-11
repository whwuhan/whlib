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
            Eigen::MatrixXd UVs;//纹理坐标
            Eigen::MatrixXd normals;//法线

            Eigen::MatrixXi verticesIndices;  //多边形面片索引
            Eigen::MatrixXi UVIndices;//纹理坐标索引
            Eigen::MatrixXi normalsIndices;//纹理坐标索引
            
            //适配Coolender
            unsigned int VAO;//mesh的VAO
            unsigned int VBO;//mesh的VBO
            unsigned int EBO;//mesh的EBO
            bool show;//是否显示
            glm::vec4 faceColor;//面片颜色
            glm::vec4 pointAndLineColor;//点和线的颜色
            glm::mat4 model;//model矩阵
            float scale;//缩放大小
            float transX;//X轴的位移
            float transY;//Y轴的位移
            float transZ;//Z轴的位移
            float rotateX;//绕X轴旋转
            float rotateY;//绕Y轴旋转
            float rotateZ;//绕Z轴旋转
            float pointSize;//点的大小






            //std::set<wh::basic::Edge> edges;//边集合
            //构造函数
            PolygonMesh();
            PolygonMesh(Eigen::MatrixXd &vertices, Eigen::MatrixXi &verticesIndices);

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