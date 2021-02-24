#pragma once
/**
 * 
 * 关于体素的算法
 * 
*/
#include <basic/cube.h>
#include <set>
namespace wh
{
    namespace alg
    {
        class VoxelAlg
        {
        public:
            //获取实心的体素
            static void getSolidVoxelIndex
            (
                wh::basic::Cube &boundingBox,               //boundingbox
                std::vector<int>& modelVoxelIndex           //模型的体素的Index
            );
        // private:
            //通过体素的index获取体素的xyz坐标
            static Eigen::RowVector3i getVoxelXYZ(int i, wh::basic::Cube &boundingBox);
            //通过体素XYZ坐标获取index
            static int getVoxelIndex(Eigen::RowVector3i voxelXYZ, wh::basic::Cube &boundingBox);
            //判断是否是内部模型内部体素
            static bool isInnerVoxel
            (
                Eigen::RowVector3i& voxelXYZ, 
                std::vector<int>& modelVoxelIndex, 
                wh::basic::Cube &boundingBox
            );
        };
    }
}