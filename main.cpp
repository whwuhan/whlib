#include <whlib.h>
#include <iostream>

// int main(){
//     //读取点云
//     wh::basic::PointCloud point_cloud;
//     wh::utils::io::load_point_cloud_obj("model/BirdM2.obj", &point_cloud);

//     //获取点云boundingbox，返回一个Cube
//     wh::basic::Cube bounding_box = point_cloud.get_bounding_box();
//     //点云归一化，返回归一化后的boundingbox
//     wh::basic::Cube nor_bounding_box = point_cloud.get_normalized_point_cloud();
//     //保存boundingbox
//     wh::utils::io::save_cube_wireframe_obj("model/bounding_box.obj", &bounding_box);
//     wh::utils::io::save_cube_wireframe_obj("model/nor_bounding_box.obj", &nor_bounding_box);
//     //保存点云
//     wh::utils::io::save_point_cloud_obj("model/BirdM3.obj", &point_cloud);

//     // //获取点云的体素网格模型并保存
//     float leaf_size = 0.02;//体素的大小
//     //获取体素对应的Cube 函数参数：1点云对应bounding box，体素划分的大小
//     std::set<wh::basic::Cube> pointCloudVoxel = point_cloud.voxelization(nor_bounding_box, leaf_size);
//     //将体素保存为线框
//     wh::utils::io::save_cube_wireframes_obj("model/pointCloudVoxel.obj", pointCloudVoxel);
//     //将体素保存为mesh
//     wh::utils::io::save_tri_cube_meshes_obj("model/pointCloudVoxel.obj", pointCloudVoxel);

//     return 0;
// }

#include <whlib.h>
int main(){
    //读取点云
    wh::basic::PointCloud point_cloud;
    wh::utils::io::load_point_cloud_obj("model/BirdM2.obj", &point_cloud);

    //获取点云bounding_box，返回一个Cube
    wh::basic::Cube bounding_box = point_cloud.get_bounding_box();
    //点云归一化，返回归一化后的bounding_box
    wh::basic::Cube nor_point_cloud_bounding_box = point_cloud.get_normalized_point_cloud();
    //保存bounding_box
    wh::utils::io::save_cube_wireframe_obj("model/bounding_box.obj", &bounding_box);
    wh::utils::io::save_cube_wireframe_obj("model/nor_point_cloud_bounding_box.obj", &nor_point_cloud_bounding_box);
    //保存点云
    wh::utils::io::save_point_cloud_obj("model/BirdM3.obj", &point_cloud);

    //获取点云的体素网格模型并保存
    float leaf_size = 0.02;//体素的大小
    //获取体素对应的Cube
    std::set<wh::basic::Cube> point_cloud_voxel = point_cloud.voxelization(nor_point_cloud_bounding_box, leaf_size);
    //将体素保存为线框
    wh::utils::io::save_cube_wireframes_obj("model/point_cloud_voxel.obj", point_cloud_voxel);
    //将体素保存为mesh
    wh::utils::io::save_tri_cube_meshes_obj("model/point_cloud_voxel.obj", point_cloud_voxel);
    return 0;
}

//运行程序
/**
pointsAmount:26392
edgesAmount:39588
# whlib cube wireframes obj file
# Tue Feb 23 21:00:29 2021
o 05
Load Skeleton Successfully!
Save Skeleton Successfully!
*/