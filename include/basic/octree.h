//八叉树结构体
#pragma once
#include <basic/face.h>
#include <Eigen/Dense>
#include <set>
#include <vector>
namespace wh{
    namespace basic{
        //声明
        //八叉树类型枚举
        enum OctreeNodeType{
            ROOT,               //根
            BOTTOM_LEFT_FRONT,  // 1
            BOTTOM_RIGHT_FRONT, // 2
            BOTTOM_LEFT_BACK,   // 3
            BOTTOM_RIGHT_BACK,  // 4
            TOP_LEFT_FRONT,     // 5
            TOP_RIGHT_FRONT,    // 6
            TOP_LEFT_BACK,      // 7
            TOP_RIGHT_BACK,     // 8
            UN_DEFINE           // 9
        };

        //八叉树声明
        template <typename T>
        class OctreeNode{
        public:
            T data;                        //节点数据(都用set存储)
            OctreeNodeType octree_node_type; //节点类型
            Eigen::RowVector3d position;   //节点中心的位置
            //节点长宽高
            double x;
            double y;
            double z;

            //节点的子结点
            OctreeNode<T> *top_left_front;
            OctreeNode<T> *top_left_back;
            OctreeNode<T> *top_right_front;
            OctreeNode<T> *top_right_back;
            OctreeNode<T> *bottom_left_front;
            OctreeNode<T> *bottom_left_back;
            OctreeNode<T> *bottom_right_front;
            OctreeNode<T> *bottom_right_back;

            unsigned int level;    //当前层级
            unsigned int max_level; //最大层级

            OctreeNode();
            OctreeNode(
                T data,                        //节点数据
                OctreeNodeType octree_node_type, //节点类型
                Eigen::RowVector3d position,   //节点位置
                double x,                      //节点x长度
                double y,                      //节点y长度
                double z,                      //节点z长度
                // OctreeNode <T>*top_left_front,
                // OctreeNode <T>*top_left_back,
                // OctreeNode <T>*top_right_front,
                // OctreeNode <T>*top_right_back,
                // OctreeNode <T>*bottom_left_front,
                // OctreeNode <T>*bottom_left_back,
                // OctreeNode <T>*bottom_right_front,
                // OctreeNode <T>*bottom_right_back,
                unsigned int level,     //节点层数
                unsigned int max_level);//最大层数

            //创建polygon_mesh的八叉树
            void create_octree(int level, Eigen::MatrixXd &vertices);

            //数据插入八叉树
            OctreeNode<T> *insert(T &data);

            //获取某个节点(范围)中的数据
            T get_data_at(
                const Eigen::RowVector3d &position,
                double x, double y, double z,
                Eigen::MatrixXd &vertices);

            //判断面片是否在包含在节点内
            bool is_contained(T &data);
        };

        //============实现
        //无参构造函数
        template <typename T>
        OctreeNode<T>::OctreeNode()
            : data(), octree_node_type(UN_DEFINE), position(),
              x(0.0), y(0.0), z(0.0),
              top_left_front(nullptr),
              top_left_back(nullptr),
              top_right_front(nullptr),
              top_right_back(nullptr),
              bottom_left_front(nullptr),
              bottom_left_back(nullptr),
              bottom_right_front(nullptr),
              bottom_right_back(nullptr),
              level(0),
              max_level(0){}

        //polygon_mesh 八叉树 data是面片索引(Eigen::MatrixXi faces)，顶点信息不保存在octree中，在创建octree的时候传入
        template <typename T>
        OctreeNode<T>::OctreeNode(
            T data,
            OctreeNodeType octree_node_type,
            Eigen::RowVector3d position,
            double x,
            double y,
            double z,
            // OctreeNode <T>*top_left_front,
            // OctreeNode <T>*top_left_back,
            // OctreeNode <T>*top_right_front,
            // OctreeNode <T>*top_right_back,
            // OctreeNode <T>*bottom_left_front,
            // OctreeNode <T>*bottom_left_back,
            // OctreeNode <T>*bottom_right_front,
            // OctreeNode <T>*bottom_right_back,
            unsigned int level,
            unsigned int max_level) : data(data),
                                     octree_node_type(octree_node_type),
                                     position(position),
                                     x(x),
                                     y(y),
                                     z(z),
                                     // top_left_front(top_left_front),
                                     // top_left_back(top_left_back),
                                     // top_right_front(top_right_front),
                                     // top_right_back(top_right_back),
                                     // bottom_left_front(bottom_left_front),
                                     // bottom_left_back(bottom_left_back),
                                     // bottom_right_front(bottom_right_front),
                                     // bottom_right_back(bottom_right_back),
                                     level(level),
                                     max_level(max_level)
        {}

        //判断面片是否在包含在节点内
        template <typename T>
        bool OctreeNode<T>::is_contained(T &data){
            //获取限制
            double xMin = position[0] - x / 2.0;
            double xMax = position[0] + x / 2.0;
            double yMin = position[1] - y / 2.0;
            double yMax = position[1] + y / 2.0;
            double zMin = position[2] - z / 2.0;
            double zMax = position[2] + z / 2.0;
        }

        //获取某个节点(范围)中的mesh数据
        template <typename T>
        T OctreeNode<T>::get_data_at(
            const Eigen::RowVector3d &position,
            double x, double y, double z, Eigen::MatrixXd &vertices){
            //获取限制
            double xMin = position[0] - x / 2.0;
            double xMax = position[0] + x / 2.0;
            double yMin = position[1] - y / 2.0;
            double yMax = position[1] + y / 2.0;
            double zMin = position[2] - z / 2.0;
            double zMax = position[2] + z / 2.0;

            std::set<wh::basic::Face> faces;
            for (int i = 0; i < data.rows(); i++){
                if (
                    (
                        vertices(data.row(i)[0], 0) > xMin && vertices(data.row(i)[0], 0) < xMax &&
                        vertices(data.row(i)[0], 1) > yMin && vertices(data.row(i)[0], 1) < yMax &&
                        vertices(data.row(i)[0], 2) > zMin && vertices(data.row(i)[0], 2) < zMax) ||
                    (
                        vertices(data.row(i)[1], 0) > xMin && vertices(data.row(i)[1], 0) < xMax &&
                        vertices(data.row(i)[1], 1) > yMin && vertices(data.row(i)[1], 1) < yMax &&
                        vertices(data.row(i)[1], 2) > zMin && vertices(data.row(i)[1], 2) < zMax) ||
                    (
                        vertices(data.row(i)[2], 0) > xMin && vertices(data.row(i)[2], 0) < xMax &&
                        vertices(data.row(i)[2], 1) > yMin && vertices(data.row(i)[2], 1) < yMax &&
                        vertices(data.row(i)[2], 2) > zMin && vertices(data.row(i)[2], 2) < zMax)

                ){
                    faces.insert(wh::basic::Face(data.row(i)));
                } //end if
            }
            //返回某个节点的面片数据
            int size = faces.size();
            T data(size, 3);
            for (auto iter = faces.begin(); iter != faces.end(); iter++){
                for (int i = 0; i < size; i++){
                    data(i, 0) = iter->fir_ver_index;
                    data(i, 1) = iter->sec_ver_index;
                    data(i, 2) = iter->thd_ver_index;
                }
            }
            return data;
        }

        //创建polygon_mesh八叉树
        template <typename T>
        void OctreeNode<T>::create_octree(int level, Eigen::MatrixXd &vertices){
            //递归地进行八叉树空间划分，直到最大深度
            if (level == max_level){
                return;
            }
            //创建子节点
            //获取子节点的位置
            Eigen::RowVector3d BLF_pos(position[0] - x / 4.0, position[1] - y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d BRF_pos(position[0] + x / 4.0, position[1] - y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d BLB_pos(position[0] - x / 4.0, position[1] - y / 4.0, position[2] - z / 4.0);
            Eigen::RowVector3d BRB_pos(position[0] + x / 4.0, position[1] - y / 4.0, position[2] - z / 4.0);

            Eigen::RowVector3d TLF_pos(position[0] - x / 4.0, position[1] + y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d TRF_pos(position[0] + x / 4.0, position[1] + y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d TLB_pos(position[0] - x / 4.0, position[1] + y / 4.0, position[2] - z / 4.0);
            Eigen::RowVector3d TRB_pos(position[0] + x / 4.0, position[1] + y / 4.0, position[2] - z / 4.0);

            //获取子节点数据
            T BLF_data = get_data_at(BLF_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BRF_data = get_data_at(BRF_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BLB_data = get_data_at(BLB_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BRB_data = get_data_at(BRB_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TLF_data = get_data_at(TLF_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TRF_data = get_data_at(TRF_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TLB_data = get_data_at(TLB_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TRB_data = get_data_at(TRB_pos, x / 2.0, y / 2.0, z / 2.0, vertices);
            OctreeNode<T> *bottom_left_front_node = new OctreeNode<T>(BLF_data, BOTTOM_LEFT_FRONT, BLF_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *bottom_right_front_node = new OctreeNode<T>(BRF_data, BOTTOM_RIGHT_FRONT, BRF_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *bottom_left_back_node = new OctreeNode<T>(BLB_data, BOTTOM_LEFT_BACK, BLB_data, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *bottom_right_back_node = new OctreeNode<T>(BRB_data, BOTTOM_RIGHT_BACK, BRB_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *top_left_front_node = new OctreeNode<T>(TLF_data, TOP_LEFT_FRONT, TLF_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *top_right_front_node = new OctreeNode<T>(TRF_data, TOP_RIGHT_FRONT, TRF_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *top_left_back_node = new OctreeNode<T>(TLB_data, TOP_LEFT_BACK, TLB_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);
            OctreeNode<T> *top_right_back_node = new OctreeNode<T>(TRB_data, TOP_RIGHT_BACK, TRB_pos, x / 2.0, y / 2.0, z / 2.0, level + 1, max_level);

            //递归构造
            bottom_left_front_node->create_octree(level + 1);
            bottom_right_front_node->create_octree(level + 1);
            bottom_left_back_node->create_octree(level + 1);
            bottom_right_back_node->create_octree(level + 1);
            top_left_front_node->create_octree(level + 1);
            top_right_front_node->create_octree(level + 1);
            top_left_back_node->create_octree(level + 1);
            top_right_back_node->create_octree(level + 1);
        }
    } // end namespace basic
} // end namespace wh
