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
        };
    }
}