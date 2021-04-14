#include <basic/cube.h>
using namespace std;
using namespace Eigen;
using namespace wh::basic;

//构造函数
Cube::Cube():
position(0.0, 0.0, 0.0), 
side_len(0.0), 
vertices(), 
x(0.0), 
y(0.0), 
z(0.0),
x_voxel_size(0),
y_voxel_size(0),
z_voxel_size(0){}

//用所有的点表示cube
Cube::Cube(MatrixXd vertices):
vertices(vertices),
x_voxel_size(0),
y_voxel_size(0),
z_voxel_size(0){
    RowVector3d center = vertices.colwise().sum(); //colwise()按照矩阵每一列的方向上排列 这里相当于每一行相加
    position = center / vertices.rows();

    RowVector3d temp = vertices.row(1) - vertices.row(0);
    x = sqrt(temp.dot(temp));

    temp = vertices.row(3) - vertices.row(0);
    y = sqrt(temp.dot(temp));

    temp = vertices.row(7) - vertices.row(0);
    z = sqrt(temp.dot(temp));

    if ((x - y < 0.00000001) && (y - z < 0.00000001)){
        side_len = x;
    }
}

//中心位置和边长表示Cube
Cube::Cube(RowVector3d position, double side_len):
position(position), 
side_len(side_len),
x_voxel_size(0),
y_voxel_size(0),
z_voxel_size(0){
    double half_size = side_len / 2.0;
    vertices.resize(8, 3);

    vertices.row(0) = RowVector3d(position[0] - half_size, position[1] - half_size, position[2] - half_size);
    vertices.row(1) = RowVector3d(position[0] + half_size, position[1] - half_size, position[2] - half_size);
    vertices.row(2) = RowVector3d(position[0] + half_size, position[1] + half_size, position[2] - half_size);
    vertices.row(3) = RowVector3d(position[0] - half_size, position[1] + half_size, position[2] - half_size);
    vertices.row(4) = RowVector3d(position[0] - half_size, position[1] + half_size, position[2] + half_size);
    vertices.row(5) = RowVector3d(position[0] + half_size, position[1] + half_size, position[2] + half_size);
    vertices.row(6) = RowVector3d(position[0] + half_size, position[1] - half_size, position[2] + half_size);
    vertices.row(7) = RowVector3d(position[0] - half_size, position[1] - half_size, position[2] + half_size);
    // position_side_len_to_vertices();

    x = side_len;
    y = side_len;
    z = side_len;
}

//长方体
Cube::Cube(Eigen::RowVector3d position, double x, double y, double z) : position(position), x(x), y(y), z(z){
    side_len = 0.0;
    position_side_len_to_vertices_cuboid();
}

//运算符重载
bool Cube::operator<(const Cube &cube) const{
    if (position[0] < cube.position[0]){
        return true;
    }
    if (position[1] < cube.position[1]){
        return true;
    }
    if (position[2] < cube.position[2]){
        return true;
    }
    return false;
}

//显示信息
void Cube::show_inf(){
    cout << "position:" << position << endl;
    cout << "side_len:" << side_len << endl;
    cout << "x:" << x << " y:" << y << " z:" << z << endl;
}

//显示信息
void Cube::show_inf() const{
    cout << "position:" << position << endl;
    cout << "side_len:" << side_len << endl;
    cout << "x:" << x << " y:" << y << " z:" << z << endl;
}

//将position side_len转化成顶点表达式
void Cube::position_side_len_to_vertices(){
    vertices.resize(8, 3);
    double half_size = side_len / 2.0;
    vertices.row(0) = RowVector3d(position[0] - half_size, position[1] - half_size, position[2] - half_size);
    vertices.row(1) = RowVector3d(position[0] + half_size, position[1] - half_size, position[2] - half_size);
    vertices.row(2) = RowVector3d(position[0] + half_size, position[1] + half_size, position[2] - half_size);
    vertices.row(3) = RowVector3d(position[0] - half_size, position[1] + half_size, position[2] - half_size);
    vertices.row(4) = RowVector3d(position[0] - half_size, position[1] + half_size, position[2] + half_size);
    vertices.row(5) = RowVector3d(position[0] + half_size, position[1] + half_size, position[2] + half_size);
    vertices.row(6) = RowVector3d(position[0] + half_size, position[1] - half_size, position[2] + half_size);
    vertices.row(7) = RowVector3d(position[0] - half_size, position[1] - half_size, position[2] + half_size);
}

//将position side_len转化成顶点表达式
void Cube::position_side_len_to_vertices_cuboid(){
    vertices.resize(8, 3);
    vertices.row(0) = position + RowVector3d(-x / 2.0, -y / 2.0, -z / 2.0);
    vertices.row(1) = position + RowVector3d(x / 2.0, -y / 2.0, -z / 2.0);
    vertices.row(2) = position + RowVector3d(x / 2.0, y / 2.0, -z / 2.0);
    vertices.row(3) = position + RowVector3d(-x / 2.0, y / 2.0, -z / 2.0);
    vertices.row(4) = position + RowVector3d(-x / 2.0, y / 2.0, z / 2.0);
    vertices.row(5) = position + RowVector3d(x / 2.0, y / 2.0, z / 2.0);
    vertices.row(6) = position + RowVector3d(x / 2.0, -y / 2.0, z / 2.0);
    vertices.row(7) = position + RowVector3d(-x / 2.0, -y / 2.0, z / 2.0);
}

//Cube的体素化
//leaf_size:体素化立方体边长
vector<Cube> Cube::voxelization(double leaf_size){
    vector<Cube> res;
    //Cube的边长不够细分
    if (x <= leaf_size || y <= leaf_size || z <= leaf_size)
        return res;

    //获取xyz方向细分的个数
    int x_amount = x / leaf_size;
    int y_amount = y / leaf_size;
    int z_amount = z / leaf_size;

    //如果恰好能分割成 x_amount  y_amount  z_amount就不增加数量了
    if (x_amount < x / leaf_size){
        x_amount++;
    }
    if (y_amount < y / leaf_size)
    {
        y_amount++;
    }
    if (z_amount < z / leaf_size)
    {
        z_amount++;
    }
    x_voxel_size = x_amount;
    y_voxel_size = y_amount;
    z_voxel_size = z_amount;
    
    double half_size = leaf_size / 2.0;

    //计算细分后的正方体
    for (int k = 0; k < z_amount; k++){
        for (int j = 0; j < y_amount; j++){
            for (int i = 0; i < x_amount; i++){
                //顺序按照xyz依次增加
                RowVector3d pos((2 * i + 1) * half_size, (2 * j + 1) * half_size, (2 * k + 1) * half_size);
                pos += vertices.row(0);
                res.push_back(Cube(pos, leaf_size));//按照xyz轴的顺序优先存放体素
            }
        }
    }
    return res;
}

// 获取Cube中心
Eigen::RowVector3d Cube::get_center(){
    return position;
}

// 获取最大边长
double Cube::get_max_side_len(){
    double res = (x > y) ? x : y;
    return (res > z) ? res : z;
}