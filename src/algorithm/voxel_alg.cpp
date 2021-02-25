#include <algorithm/voxel_alg.h>
using namespace std;
using namespace wh::basic;
using namespace wh::alg;
using namespace Eigen;
//获取实心的体素数组
void VoxelAlg::getSolidVoxelIndex(Cube &boundingBox, vector<int>& modelVoxelIndex)
{
    for(int i = 0; i < modelVoxelIndex.size(); i++)
    {
        if(modelVoxelIndex[i] == 0)
        {
            RowVector3i voxelXYZ = getVoxelXYZ(i, boundingBox);
            if(isInnerVoxel(voxelXYZ, modelVoxelIndex, boundingBox))
            {
                modelVoxelIndex[i] = 1;
            }
        }
    }
    return;
}

//通过体素的index获取体素的xyz坐标
RowVector3i VoxelAlg::getVoxelXYZ(int i, Cube &boundingBox)
{
    int xIndex = i % boundingBox.xVoxelSize;
    int yIndex = int(i / boundingBox.xVoxelSize) % boundingBox.yVoxelSize;
    int zIndex = int(i / (boundingBox.xVoxelSize * boundingBox.yVoxelSize));
    return RowVector3i(xIndex, yIndex, zIndex);
}

//通过体素XYZ坐标获取index
int VoxelAlg::getVoxelIndex(RowVector3i voxelXYZ, Cube &boundingBox)
{
    return voxelXYZ[2] * boundingBox.xVoxelSize * boundingBox.yVoxelSize + \
    voxelXYZ[1] * boundingBox.xVoxelSize + \
    voxelXYZ[0];
}

//判断某个体素XYZ是否在模型内部
bool VoxelAlg::isInnerVoxel(RowVector3i& voxelXYZ, vector<int>& modelVoxelIndex, Cube &boundingBox)
{
    //判断x正方向是否有体素
    bool xPosFlag = false;
    RowVector3i compareVoxelXYZ(voxelXYZ);//要与voxelXYZ比较的体素
    for(int x = compareVoxelXYZ[0] + 1; x < boundingBox.xVoxelSize; x++)
    {
        compareVoxelXYZ[0] = x;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            xPosFlag = true;
            break;
        }
    }

    //判断x负方向是否有体素
    bool xNegFlag = false;
    compareVoxelXYZ = voxelXYZ;//要与voxelXYZ比较的体素
    for(int x = compareVoxelXYZ[0] - 1; x >= 0; x--)
    {
        compareVoxelXYZ[0] = x;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            xNegFlag = true;
            break;
        }
    }

    //判断y正方向是否有体素
    bool yPosFlag = false;
    compareVoxelXYZ = voxelXYZ;
    for(int y = compareVoxelXYZ[1] + 1; y < boundingBox.yVoxelSize; y++)
    {
        compareVoxelXYZ[1] = y;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            yPosFlag = true;
            break;
        }
    }

    //判断y负方向是否有体素
    bool yNegFlag = false;
    compareVoxelXYZ = voxelXYZ;
    for(int y = compareVoxelXYZ[1] - 1; y >= 0; y--)
    {
        compareVoxelXYZ[1] = y;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            yNegFlag = true;
            break;
        }
    }

    //判断z正方向是否有体素
    bool zPosFlag = false;
    compareVoxelXYZ = voxelXYZ;
    for(int z = compareVoxelXYZ[2] + 1; z < boundingBox.zVoxelSize; z++)
    {
        compareVoxelXYZ[2] = z;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            zPosFlag = true;
            break;
        }
    }

    //判断z负方向是否有体素
    bool zNegFlag = false;
    compareVoxelXYZ = voxelXYZ;
    for(int z = compareVoxelXYZ[2] - 1; z >= 0; z--)
    {
        compareVoxelXYZ[2] = z;
        int compareVoxelIndex = getVoxelIndex(compareVoxelXYZ, boundingBox);//获取要比较的体素index
        if(1 == modelVoxelIndex[compareVoxelIndex])
        {
            zNegFlag = true;
            break;
        }
    }
    //如果在xyz方向上都有体素，说明是模型内部体素
    return xPosFlag && xNegFlag && yPosFlag && yNegFlag && zPosFlag && zNegFlag;
}


