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
            static void getSolidVoxel
            (
                std::vector<wh::basic::Cube>& boundingBoxVoxel, //包围盒的体素
                std::set<wh::basic::Cube>& modelVoxel           //模型的体素
            );
        };
    }
}