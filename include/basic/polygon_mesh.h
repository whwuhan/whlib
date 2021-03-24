#pragma once
#include <Eigen/Dense>
#include <basic/edge.h>
#include <basic/face.h>
#include <set>
#include <map>
#include <glm/glm.hpp>
namespace wh{
    namespace basic{
        typedef struct PolygonMesh {
            Eigen::MatrixXd vertices; //多边型顶点
            Eigen::MatrixXd uvs;//纹理坐标
            Eigen::MatrixXd normals;//法线

            Eigen::MatrixXi vertices_indices;  //多边形面片索引
            Eigen::MatrixXi uv_indices;//纹理坐标索引
            Eigen::MatrixXi normals_indices;//纹理坐标索引
            
            //适配Coolender
            unsigned int VAO;//mesh的VAO
            unsigned int VBO;//mesh的VBO
            unsigned int EBO;//mesh的EBO
            bool show;//是否显示
            glm::vec4 face_color;//面片颜色
            glm::vec4 point_and_line_color;//点和线的颜色
            glm::mat4 model;//model矩阵
            float scale;//缩放大小
            float trans_x;//X轴的位移
            float trans_y;//Y轴的位移
            float trans_z;//Z轴的位移
            float rotate_x;//绕X轴旋转
            float rotate_y;//绕Y轴旋转
            float rotate_z;//绕Z轴旋转
            float point_size;//点的大小






            //std::set<wh::basic::Edge> edges;//边集合
            //构造函数
            PolygonMesh();
            PolygonMesh(Eigen::MatrixXd &vertices, Eigen::MatrixXi &vertices_indices);

            //mesh 细分
            void tri_mesh_subdivision(); //三角面片细分
            void loop_subdivision();     //loop细分 TODO

            //=======其他功能
            //生成边集
            std::set<wh::basic::Edge> creat_unordered_edges(); //无序边
            std::set<wh::basic::Edge> creat_ordered_edges();   //有序边

            //找到边的相邻面片
            std::map<wh::basic::Edge, std::set<wh::basic::Face> > find_edge_near_faces(std::set<wh::basic::Edge> &edges);
        } POLYGON_MESH;
    } // namespace basic
} // namespace wh   