#include <whlib.h>
#include <iostream>

int main(){
    //读取点云
    wh::basic::PointCloud pointCloud;
    wh::utils::io::loadPointCloudObj("model/BirdM2.obj", &pointCloud);

    //获取点云boundingbox，返回一个Cube
    wh::basic::Cube boundingBox = pointCloud.getBoundingBox();
    //点云归一化，返回归一化后的boundingbox
    wh::basic::Cube norPointCloudBoundingBox = pointCloud.getNormalizedPointCloud();
    //保存boundingbox
    wh::utils::io::saveCubeWireframeObj("model/boundingBox.obj", &boundingBox);
    wh::utils::io::saveCubeWireframeObj("model/norPointCloudBoundingBox.obj", &norPointCloudBoundingBox);
    //保存点云
    wh::utils::io::savePointCloudObj("model/BirdM3.obj", &pointCloud);

    // //获取点云的体素网格模型并保存
    float leafSize = 0.02;//体素的大小
    //获取体素对应的Cube 函数参数：1点云对应bounding box，体素划分的大小
    std::set<wh::basic::Cube> pointCloudVoxel = pointCloud.voxelization(norPointCloudBoundingBox, leafSize);
    //将体素保存为线框
    wh::utils::io::saveCubeWireframesObj("model/pointCloudVoxelWireframe.obj", pointCloudVoxel);
    //将体素保存为mesh
    wh::utils::io::saveTriCubeMeshesObj("model/pointCloudVoxelMesh.obj", pointCloudVoxel);
    
    // Cube boundingBox = pointCloud.getBoundingBox();
    // double leafSize = 0.02;
    // //获取点云体素
    // set<Cube> pointCloudVoxel =
    // pointCloud.voxelization(boundingBox, leafSize);
    
    // vector<int> voxelIndex = pointCloud.getVoxelIndex(boundingBox,leafSize);
    // vector<Cube> cubeVoxel = boundingBox.voxelization(leafSize);
    // vector<int> voxelIndexTemp = voxelIndex;
    // VoxelAlg::getSolidVoxelIndex(boundingBox, voxelIndex);
    // //实心体素
    // vector<Cube> solidVoxel;
    // for(int i = 0; i < voxelIndex.size(); i++)
    // {
    //     if(voxelIndex[i] == 1)
    //     {
    //         solidVoxel.push_back(cubeVoxel[i]);
    //     }
    // }

    // saveCubeWireframesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_solid_vox_wf_0.02_2.obj",
    //     solidVoxel
    // );

    // saveCubeWireframesObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_vox_wf_0.02_2.obj",
    //     pointCloudVoxel
    // );

    // //读入多边形网格
    // wh::basic::PolygonMesh mesh;
    // wh::utils::io::loadPolygonMeshObj("model/BirdM2_vox_0.02_2.obj", &mesh);
    // //存放多边形网格
    // wh::utils::io::savePolygonMeshObj("model/BirdM2_vox_0.02_3.obj", &mesh);
    
    // mesh.triMeshSubdivision();

    // savePolygonMeshObj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_vox_0.02_subdiv_2.obj",
    //     &mesh
    // );

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

    // PolygonMesh mesh;
    // loadPolygonMeshObj("model/african_head.obj", &mesh);

    //savePolygonMeshObj("model/BirdMeshTest.obj", &mesh);

    //读取骨架线
    // wh::basic::Skeleton skel;
    // wh::utils::io::loadSkeletonObj("model/tree4_84336_nor_bb_wf_vox_0.05.obj", &skel);
    // // //存放骨架
    // wh::utils::io::saveSkeletonObj("model/tree4_84336_nor_bb_wf_vox_0.05.obj", &skel);
    return 0;
}