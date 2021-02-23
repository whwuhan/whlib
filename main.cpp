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
        "/Users/wuhan/wuhan/CodingSpace/Coolender/tree4_84336_nor.obj",
        &pointcloud
    );
    Cube boundingBox = pointcloud.getBoundingBox();
    double leafSize = 0.05;
    //获取点云体素
    set<Cube> pointCloudVoxel =
    pointcloud.voxelization(boundingBox, leafSize);
    
    saveTriCubeMeshesObj
    (
        "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_vox_0.05.obj",
        pointCloudVoxel
    );

    saveCubeWireframesObj
    (
        "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_wf_vox_0.05.obj",
        pointCloudVoxel
    );

    vector<Cube> boundingBoxVoxel = boundingBox.voxelization(0.05);

    saveTriCubeMeshesObj
    (
        "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_vox_0.05.obj",
        boundingBoxVoxel
    );
    
    return 0;
}