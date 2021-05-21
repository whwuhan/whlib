#include <algorithm/voxel_alg.h>
using namespace std;
using namespace wh::basic;
using namespace wh::alg;
using namespace Eigen;
//获取实心的体素数组
void VoxelAlg::get_solid_voxel_index(Cube &bounding_box, vector<int>& model_voxel_index)
{
    for(int i = 0; i < model_voxel_index.size(); i++)
    {
        if(model_voxel_index[i] == 0)
        {
            RowVector3i voxel_xyz = get_voxel_xyz(i, bounding_box);
            if(is_inner_voxel(voxel_xyz, model_voxel_index, bounding_box))
            {
                model_voxel_index[i] = 1;
            }
        }
    }
    return;
}

//通过体素的index获取体素的xyz坐标
RowVector3i VoxelAlg::get_voxel_xyz(int i, Cube &bounding_box)
{
    int x_index = i % bounding_box.x_voxel_size;
    int y_index = int(i / bounding_box.x_voxel_size) % bounding_box.y_voxel_size;
    int z_index = int(i / (bounding_box.x_voxel_size * bounding_box.y_voxel_size));
    return RowVector3i(x_index, y_index, z_index);
}

//通过体素XYZ坐标获取index
int VoxelAlg::get_voxel_index(RowVector3i voxel_xyz, Cube &bounding_box)
{
    return voxel_xyz[2] * bounding_box.x_voxel_size * bounding_box.y_voxel_size + \
    voxel_xyz[1] * bounding_box.x_voxel_size + \
    voxel_xyz[0];
}

//判断某个体素XYZ是否在模型内部
bool VoxelAlg::is_inner_voxel(RowVector3i& voxel_xyz, vector<int>& model_voxel_index, Cube &bounding_box)
{
    //判断x正方向是否有体素
    bool x_pos_flag = false;
    RowVector3i compare_voxel_xyz(voxel_xyz);//要与voxel_xyz比较的体素
    for(int x = compare_voxel_xyz[0] + 1; x < bounding_box.x_voxel_size; x++)
    {
        compare_voxel_xyz[0] = x;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            x_pos_flag = true;
            break;
        }
    }

    //判断x负方向是否有体素
    bool x_neg_flag = false;
    compare_voxel_xyz = voxel_xyz;//要与voxel_xyz比较的体素
    for(int x = compare_voxel_xyz[0] - 1; x >= 0; x--)
    {
        compare_voxel_xyz[0] = x;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            x_neg_flag = true;
            break;
        }
    }

    //判断y正方向是否有体素
    bool y_pos_flag = false;
    compare_voxel_xyz = voxel_xyz;
    for(int y = compare_voxel_xyz[1] + 1; y < bounding_box.y_voxel_size; y++)
    {
        compare_voxel_xyz[1] = y;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            y_pos_flag = true;
            break;
        }
    }

    //判断y负方向是否有体素
    bool y_neg_flag = false;
    compare_voxel_xyz = voxel_xyz;
    for(int y = compare_voxel_xyz[1] - 1; y >= 0; y--)
    {
        compare_voxel_xyz[1] = y;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            y_neg_flag = true;
            break;
        }
    }

    //判断z正方向是否有体素
    bool z_pos_flag = false;
    compare_voxel_xyz = voxel_xyz;
    for(int z = compare_voxel_xyz[2] + 1; z < bounding_box.z_voxel_size; z++)
    {
        compare_voxel_xyz[2] = z;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            z_pos_flag = true;
            break;
        }
    }

    //判断z负方向是否有体素
    bool z_neg_flag = false;
    compare_voxel_xyz = voxel_xyz;
    for(int z = compare_voxel_xyz[2] - 1; z >= 0; z--)
    {
        compare_voxel_xyz[2] = z;
        int compare_voxel_index = get_voxel_index(compare_voxel_xyz, bounding_box);//获取要比较的体素index
        if(1 == model_voxel_index[compare_voxel_index])
        {
            z_neg_flag = true;
            break;
        }
    }
    //如果在xyz方向上都有体素，说明是模型内部体素
    return x_pos_flag && x_neg_flag && y_pos_flag && y_neg_flag && z_pos_flag && z_neg_flag;
}


