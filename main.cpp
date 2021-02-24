#include <whlib.h>
#include <iostream>
using namespace std;
using namespace wh::basic;
using namespace wh::alg;
using namespace wh::utils::io;

int main()
{
    PointCloud pointcloud;
    //读取点云
    loadPointCloudObj
    (
        "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2.obj",
        &pointcloud
    );
    Cube boundingBox = pointcloud.getBoundingBox();
    double leafSize = 0.05;
    //获取点云体素
    set<Cube> pointCloudVoxel =
    pointcloud.voxelization(boundingBox, leafSize);
    
    vector<int> voxelIndex = pointcloud.getVoxelIndex(boundingBox,leafSize);
    vector<Cube> cubeVoxel = boundingBox.voxelization(leafSize);
    vector<int> voxelIndexTemp = voxelIndex;
    VoxelAlg::getSolidVoxelIndex(boundingBox, voxelIndex);
    //实心体素
    vector<Cube> solidVoxel;
    for(int i = 0; i < voxelIndex.size(); i++)
    {
        if(voxelIndex[i] == 1)
        {
            solidVoxel.push_back(cubeVoxel[i]);
        }
    }

    for(int i = 0; i < voxelIndexTemp.size(); i++)
    {
        if(voxelIndexTemp[i] != voxelIndex[i])
        {
            cout << i << endl;
        }
    }
    saveTriCubeMeshesObj
    (
        "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_solid_vox_0.05_2.obj",
        solidVoxel
    );

    // saveTriCubeMeshesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_vox_0.05_2.obj",
    //     res
    // );

    // saveTriCubeMeshesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_vox_0.05.obj",
    //     pointCloudVoxel
    // );

    // saveCubeWireframesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_wf_vox_0.05.obj",
    //     pointCloudVoxel
    // );

    // vector<Cube> boundingBoxVoxel = boundingBox.voxelization(0.05);

    // saveTriCubeMeshesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_vox_0.05.obj",
    //     boundingBoxVoxel
    // );

    
    return 0;
}