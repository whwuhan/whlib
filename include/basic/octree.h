//八叉树结构体
#pragma once
#include <basic/face.h>
#include <Eigen/Dense>
#include <set>
#include <vector>
namespace wh
{
    namespace basic
    {
        //声明
        //八叉树类型枚举
        enum OctreeNodeType
        {
            ROOT,               //根
            BOTTOMLEFTFRONT,  // 1
            BOTTOMRIGHTFRONT, // 2
            BOTTOMLEFTBACK,   // 3
            BOTTOMRIGHTBACK,  // 4
            TOPLEFTFRONT,     // 5
            TOPRIGHTFRONT,    // 6
            TOPLEFTBACK,      // 7
            TOPRIGHTBACK,     // 8
            UN_DEFINE           // 9
        };

        //八叉树声明
        template <typename T>
        class OctreeNode
        {
        public:
            T data;                        //节点数据(都用set存储)
            OctreeNodeType octreeNodeType; //节点类型
            Eigen::RowVector3d position;   //节点中心的位置
            //节点长宽高
            double x;
            double y;
            double z;

            //节点的子结点
            OctreeNode<T> *topLeftFront;
            OctreeNode<T> *topLeftBack;
            OctreeNode<T> *topRightFront;
            OctreeNode<T> *topRightBack;
            OctreeNode<T> *bottomLeftFront;
            OctreeNode<T> *bottomLeftBack;
            OctreeNode<T> *bottomRightFront;
            OctreeNode<T> *bottomRightBack;

            unsigned int level;    //当前层级
            unsigned int maxLevel; //最大层级

            OctreeNode();
            OctreeNode(
                T data,                        //节点数据
                OctreeNodeType octreeNodeType, //节点类型
                Eigen::RowVector3d position,   //节点位置
                double x,                      //节点x长度
                double y,                      //节点y长度
                double z,                      //节点z长度
                // OctreeNode <T>*topLeftFront,
                // OctreeNode <T>*topLeftBack,
                // OctreeNode <T>*topRightFront,
                // OctreeNode <T>*topRightBack,
                // OctreeNode <T>*bottomLeftFront,
                // OctreeNode <T>*bottomLeftBack,
                // OctreeNode <T>*bottomRightFront,
                // OctreeNode <T>*bottomRightBack,
                unsigned int level,   //节点层数
                unsigned int maxLevel //最大层数
            );

            //创建polygon_mesh的八叉树
            void createOctree(int level, Eigen::MatrixXd &vertices);

            //数据插入八叉树
            OctreeNode<T> *insert(T &data);

            //获取某个节点(范围)中的数据
            T getDataAt(const Eigen::RowVector3d &position,
                        double x, double y, double z,
                        Eigen::MatrixXd &vertices);

            //判断面片是否在包含在节点内
            bool isContained(T &data);
        };

        //============实现
        //无参构造函数
        template <typename T>
        OctreeNode<T>::OctreeNode()
            : data(), octreeNodeType(UN_DEFINE), position(),
              x(0.0), y(0.0), z(0.0),
              topLeftFront(nullptr),
              topLeftBack(nullptr),
              topRightFront(nullptr),
              topRightBack(nullptr),
              bottomLeftFront(nullptr),
              bottomLeftBack(nullptr),
              bottomRightFront(nullptr),
              bottomRightBack(nullptr),
              level(0),
              maxLevel(0)
        {
        }

        //polygon_mesh 八叉树 data是面片索引(Eigen::MatrixXi faces)，顶点信息不保存在octree中，在创建octree的时候传入
        template <typename T>
        OctreeNode<T>::OctreeNode(
            T data,
            OctreeNodeType octreeNodeType,
            Eigen::RowVector3d position,
            double x,
            double y,
            double z,
            // OctreeNode <T>*topLeftFront,
            // OctreeNode <T>*topLeftBack,
            // OctreeNode <T>*topRightFront,
            // OctreeNode <T>*topRightBack,
            // OctreeNode <T>*bottomLeftFront,
            // OctreeNode <T>*bottomLeftBack,
            // OctreeNode <T>*bottomRightFront,
            // OctreeNode <T>*bottomRightBack,
            unsigned int level,
            unsigned int maxLevel) : data(data),
                                     octreeNodeType(octreeNodeType),
                                     position(position),
                                     x(x),
                                     y(y),
                                     z(z),
                                     // topLeftFront(topLeftFront),
                                     // topLeftBack(topLeftBack),
                                     // topRightFront(topRightFront),
                                     // topRightBack(topRightBack),
                                     // bottomLeftFront(bottomLeftFront),
                                     // bottomLeftBack(bottomLeftBack),
                                     // bottomRightFront(bottomRightFront),
                                     // bottomRightBack(bottomRightBack),
                                     level(level),
                                     maxLevel(maxLevel)
        {
        }

        //判断面片是否在包含在节点内
        template <typename T>
        bool OctreeNode<T>::isContained(T &data)
        {
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
        T OctreeNode<T>::getDataAt(const Eigen::RowVector3d &position,
                                   double x, double y, double z, Eigen::MatrixXd &vertices)
        {
            //获取限制
            double xMin = position[0] - x / 2.0;
            double xMax = position[0] + x / 2.0;
            double yMin = position[1] - y / 2.0;
            double yMax = position[1] + y / 2.0;
            double zMin = position[2] - z / 2.0;
            double zMax = position[2] + z / 2.0;

            std::set<wh::basic::Face> faces;
            for (int i = 0; i < data.rows(); i++)
            {
                if (
                    (
                        vertices(data.row(i)[0], 0) > xMin && vertices(data.row(i)[0], 0) < xMax &&
                        vertices(data.row(i)[0], 1) > yMin && vertices(data.row(i)[0], 1) < yMax &&
                        vertices(data.row(i)[0], 2) > zMin && vertices(data.row(i)[0], 2) < zMax) ||
                    (vertices(data.row(i)[1], 0) > xMin && vertices(data.row(i)[1], 0) < xMax &&
                     vertices(data.row(i)[1], 1) > yMin && vertices(data.row(i)[1], 1) < yMax &&
                     vertices(data.row(i)[1], 2) > zMin && vertices(data.row(i)[1], 2) < zMax) ||
                    (vertices(data.row(i)[2], 0) > xMin && vertices(data.row(i)[2], 0) < xMax &&
                     vertices(data.row(i)[2], 1) > yMin && vertices(data.row(i)[2], 1) < yMax &&
                     vertices(data.row(i)[2], 2) > zMin && vertices(data.row(i)[2], 2) < zMax)

                )
                {
                    faces.insert(wh::basic::Face(data.row(i)));
                } //end if
            }
            //返回某个节点的面片数据
            int size = faces.size();
            T data(size, 3);
            for (auto iter = faces.begin(); iter != faces.end(); iter++)
            {
                for (int i = 0; i < size; i++)
                {
                    data(i, 0) = iter->firVerIndex;
                    data(i, 1) = iter->secVerIndex;
                    data(i, 2) = iter->thdVerIndex;
                }
            }
            return data;
        }

        //创建polygon_mesh八叉树
        template <typename T>
        void OctreeNode<T>::createOctree(int level, Eigen::MatrixXd &vertices)
        {
            //递归地进行八叉树空间划分，直到最大深度
            if (level == maxLevel)
            {
                return;
            }
            //创建子节点
            //获取子节点的位置
            Eigen::RowVector3d BLFPos(position[0] - x / 4.0, position[1] - y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d BRFPos(position[0] + x / 4.0, position[1] - y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d BLBPos(position[0] - x / 4.0, position[1] - y / 4.0, position[2] - z / 4.0);
            Eigen::RowVector3d BRBPos(position[0] + x / 4.0, position[1] - y / 4.0, position[2] - z / 4.0);

            Eigen::RowVector3d TLFPos(position[0] - x / 4.0, position[1] + y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d TRFPos(position[0] + x / 4.0, position[1] + y / 4.0, position[2] + z / 4.0);
            Eigen::RowVector3d TLBPos(position[0] - x / 4.0, position[1] + y / 4.0, position[2] - z / 4.0);
            Eigen::RowVector3d TRBPos(position[0] + x / 4.0, position[1] + y / 4.0, position[2] - z / 4.0);

            //获取子节点数据
            T BLFData = getDataAt(BLFPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BRFData = getDataAt(BRFPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BLBData = getDataAt(BLBPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T BRBData = getDataAt(BRBPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TLFData = getDataAt(TLFPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TRFData = getDataAt(TRFPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TLBData = getDataAt(TLBPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            T TRBData = getDataAt(TRBPos, x / 2.0, y / 2.0, z / 2.0, vertices);
            OctreeNode<T> *bottomLeftFront_node = new OctreeNode<T>(BLFData, BOTTOMLEFTFRONT, BLFPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *bottomRightFront_node = new OctreeNode<T>(BRFData, BOTTOMRIGHTFRONT, BRFPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *bottomLeftBack_node = new OctreeNode<T>(BLBData, BOTTOMLEFTBACK, BLBData, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *bottomRightBack_node = new OctreeNode<T>(BRBData, BOTTOMRIGHTBACK, BRBPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *topLeftFront_node = new OctreeNode<T>(TLFData, TOPLEFTFRONT, TLFPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *topRightFront_node = new OctreeNode<T>(TRFData, TOPRIGHTFRONT, TRFPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *topLeftBack_node = new OctreeNode<T>(TLBData, TOPLEFTBACK, TLBPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);
            OctreeNode<T> *topRightBack_node = new OctreeNode<T>(TRBData, TOPRIGHTBACK, TRBPos, x / 2.0, y / 2.0, z / 2.0, level + 1, maxLevel);

            //递归构造
            bottomLeftFront_node->createOctree(level + 1);
            bottomRightFront_node->createOctree(level + 1);
            bottomLeftBack_node->createOctree(level + 1);
            bottomRightBack_node->createOctree(level + 1);
            topLeftFront_node->createOctree(level + 1);
            topRightFront_node->createOctree(level + 1);
            topLeftBack_node->createOctree(level + 1);
            topRightBack_node->createOctree(level + 1);
        }
    } // namespace basic
} // namespace wh
