#include <whlib.h>
#include <iostream>

int main(){
    //读取点云
    wh::basic::PointCloud point_cloud;
    wh::utils::io::load_point_cloud_obj("model/BirdM2.obj", &point_cloud);

    //获取点云boundingbox，返回一个Cube
    wh::basic::Cube bounding_box = point_cloud.get_bounding_box();
    //点云归一化，返回归一化后的boundingbox
    wh::basic::Cube nor__bounding_box = point_cloud.get_normalized_point_cloud();
    //保存boundingbox
    wh::utils::io::save_cube_wireframe_obj("model/bounding_box.obj", &bounding_box);
    wh::utils::io::save_cube_wireframe_obj("model/nor__bounding_box.obj", &nor__bounding_box);
    //保存点云
    wh::utils::io::save_point_cloud_obj("model/BirdM3.obj", &point_cloud);

    // //获取点云的体素网格模型并保存
    float leaf_size = 0.02;//体素的大小
    //获取体素对应的Cube 函数参数：1点云对应bounding box，体素划分的大小
    std::set<wh::basic::Cube> pointCloudVoxel = point_cloud.voxelization(nor__bounding_box, leaf_size);
    //将体素保存为线框
<<<<<<< HEAD
    wh::utils::io::saveCubeWireframesObj("model/pointCloudVoxelWireframe.obj", pointCloudVoxel);
    //将体素保存为mesh
    wh::utils::io::saveTriCubeMeshesObj("model/pointCloudVoxelMesh.obj", pointCloudVoxel);
=======
    wh::utils::io::save_cube_wireframes_obj("model/pointCloudVoxel.obj", pointCloudVoxel);
    //将体素保存为mesh
    wh::utils::io::save_tri_cube_meshes_obj("model/pointCloudVoxel.obj", pointCloudVoxel);
>>>>>>> dev_format
    
    // Cube bounding_box = point_cloud.get_bounding_box();
    // double leaf_size = 0.02;
    // //获取点云体素
    // set<Cube> pointCloudVoxel =
    // point_cloud.voxelization(bounding_box, leaf_size);
    
    // vector<int> voxelIndex = point_cloud.get_voxel_index(bounding_box,leaf_size);
    // vector<Cube> cubeVoxel = bounding_box.voxelization(leaf_size);
    // vector<int> voxelIndexTemp = voxelIndex;
    // VoxelAlg::get_solid_voxel_index(bounding_box, voxelIndex);
    // //实心体素
    // vector<Cube> solidVoxel;
    // for(int i = 0; i < voxelIndex.size(); i++)
    // {
    //     if(voxelIndex[i] == 1)
    //     {
    //         solidVoxel.push_back(cubeVoxel[i]);
    //     }
    // }

    // save_cube_wireframes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_solid_vox_wf_0.02_2.obj",
    //     solidVoxel
    // );

    // save_cube_wireframes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_vox_wf_0.02_2.obj",
    //     pointCloudVoxel
    // );

    // //读入多边形网格
    // wh::basic::PolygonMesh mesh;
    // wh::utils::io::load_polygon_mesh_obj("model/BirdM2_vox_0.02_2.obj", &mesh);
    // //存放多边形网格
    // wh::utils::io::save_polygon_mesh_obj("model/BirdM2_vox_0.02_3.obj", &mesh);
    
    // mesh.tri_mesh_subdivision();

    // save_polygon_mesh_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/BirdM2_vox_0.02_subdiv_2.obj",
    //     &mesh
    // );

    // save_tri_cube_meshes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_vox_0.05_2.obj",
    //     res
    // );

    // save_tri_cube_meshes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_vox_0.05.obj",
    //     pointCloudVoxel
    // );

    // save_cube_wireframes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_wf_vox_0.05.obj",
    //     pointCloudVoxel
    // );

    // vector<Cube> boundingBoxVoxel = bounding_box.voxelization(0.05);

    // save_tri_cube_meshes_obj
    // (
    //     "/Users/wuhan/wuhan/CodingSpace/Coolender/3rdsrc/whlib/model/tree4_84336_nor_bb_vox_0.05.obj",
    //     boundingBoxVoxel
    // );

    // PolygonMesh mesh;
    // load_polygon_mesh_obj("model/african_head.obj", &mesh);

    //save_polygon_mesh_obj("model/BirdMeshTest.obj", &mesh);

    //读取骨架线
    // wh::basic::Skeleton skel;
    // wh::utils::io::load_skeleton_obj("model/tree4_84336_nor_bb_wf_vox_0.05.obj", &skel);
    // // //存放骨架
    // wh::utils::io::save_skeleton_obj("model/tree4_84336_nor_bb_wf_vox_0.05.obj", &skel);
    return 0;
}