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
            static void get_solid_voxel_index
            (
                wh::basic::Cube &bounding_box,               //boundingbox
                std::vector<int>& model_voxel_index
            );          //模型的体素的Index
        // private:
            //通过体素的index获取体素的xyz坐标
            static Eigen::RowVector3i get_voxel_xyz(int i, wh::basic::Cube &bounding_box);
            //通过体素XYZ坐标获取index
            static int get_voxel_index(Eigen::RowVector3i voxel_xyz, wh::basic::Cube &bounding_box);
            //判断是否是内部模型内部体素
            static bool is_inner_voxel
            (
                Eigen::RowVector3i& voxel_xyz, 
                std::vector<int>& model_voxel_index, 
                wh::basic::Cube& bounding_box
            );
        };
    }// end namespace alg
}// end namespace wh