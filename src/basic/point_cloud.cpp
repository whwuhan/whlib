#include <basic/point_cloud.h>
using namespace std;
using namespace Eigen;
using namespace wh::basic;
//构造函数
PointCloud::PointCloud():
points(), 
size(0),
VAO(0),
VBO(0),
show(true),
color(0.06f, 0.729f, 0.941f, 1.0f),
model(1.0f),
scale(1.0f),
transX(0.0f),
transY(0.0f),
transZ(0.0f),
rotateX(0.0f),
rotateY(0.0f),
rotateZ(0.0f),
pointSize(10.0f)
{}

PointCloud::PointCloud(const unsigned int size, const int point_size):
points(size, point_size), 
size(size) ,
VAO(0),
VBO(0),
show(true),
color(0.06f, 0.729f, 0.941f, 1.0f),
model(1.0f),
scale(1.0f),
transX(0.0f),
transY(0.0f),
transZ(0.0f),
rotateX(0.0f),
rotateY(0.0f),
rotateZ(0.0f),
pointSize(10.0f)
{}

PointCloud::PointCloud(const Eigen::MatrixXd &points):
points(points), 
size(points.rows()),
VAO(0),
VBO(0),
show(true),
color(0.06f, 0.729f, 0.941f, 1.0f),
model(1.0f),
scale(1.0f),
transX(0.0f),
transY(0.0f),
transZ(0.0f),
rotateX(0.0f),
rotateY(0.0f),
rotateZ(0.0f),
pointSize(10.0f)
{}

//重载运算符
PointCloud PointCloud::operator+(const PointCloud &point_cloud)
{
    //判断维度是否正确
    if (points.rows() != point_cloud.points.rows())
    {
        cout << "wrong row dimension." << endl;
        return PointCloud();
    }
    else if (points.cols() != point_cloud.points.cols())
    {
        cout << "wrong col dimension." << endl;
        return PointCloud();
    }
    return PointCloud(points + point_cloud.points);
}

PointCloud PointCloud::operator-(const PointCloud &point_cloud)
{
    //判断维度是否正确
    if (points.rows() != point_cloud.points.rows())
    {
        cout << "wrong row dimension." << endl;
        return PointCloud();
    }
    else if (points.cols() != point_cloud.points.cols())
    {
        cout << "wrong col dimension." << endl;
        return PointCloud();
    }
    return PointCloud(points - point_cloud.points);
}

PointCloud PointCloud::operator*(const Eigen::MatrixXd transform_matrix)
{
    //判断维度是否正确
    if (points.cols() != transform_matrix.rows())
    {
        cout << "wrong dimension." << endl;
        return PointCloud();
    }
    return PointCloud(points * transform_matrix);
}

Point3d PointCloud::operator[](const unsigned int index)
{ //判断索引是否正确
    if (index >= size)
    {
        cout << "wrong index." << endl;
        return Point3d();
    }
    return Point3d(points.row(index));
}

//重载友元<<
ostream &wh::basic::operator<<(ostream &ost, const PointCloud point_cloud)
{
    ost << point_cloud.points;
    return ost;
}

//重新确定维度，注意如果维度改变，里面的数据也可能改变
void PointCloud::resize(const unsigned int rows, const unsigned int cols)
{
    size = rows;
    points.resize(rows, cols);
}

//重新确定维度，且不会改变内部数据(Eigen似乎有bug还是会改变数据)
//如果维度比原有的维度小，那么数据不改变，相当于裁剪矩阵，如果扩张了维度，数据会是随机值
//慎用！！！
void PointCloud::conservativeResize(const unsigned int rows, const unsigned int cols)
{
    size = rows;
    points.conservativeResize(rows, cols);
}

//获取点云几何中心
RowVector3d PointCloud::getGeometricCenter()
{
    RowVector3d center = points.colwise().sum(); //colwise()按照矩阵每一列的方向上排列 这里相当于每一行相加
    return center / size;
}

//将点云放回原点
void PointCloud::getCenteredPointCloud()
{
    //获取点云几何中心
    RowVector3d center = getGeometricCenter();
    //将点云数据放回坐标原点
    for (int i = 0; i < size; i++)
    {
        points.row(i) = points.row(i) - center;
    }
}

//归一化点云
Cube PointCloud::getNormalizedPointCloud()
{
    RowVector3d max_xyz = points.colwise().maxCoeff(); //xyz坐标的最大值
    RowVector3d min_xyz = points.colwise().minCoeff(); //xyz坐标的最小值

    //包围盒中心
    RowVector3d boundingBoxCenter = (max_xyz + min_xyz) / 2.0;
    //将点云中心放置到坐标原点
    for (int i = 0; i < size; i++)
    {
        points.row(i) = points.row(i) - boundingBoxCenter;
    }

    //boundingBox的xyz轴上的边长
    RowVector3d boundingBoxSideLength = max_xyz - min_xyz;

    //获取最长边长
    double maxSideLen = boundingBoxSideLength.maxCoeff();

    //缩放大小
    double scale = maxSideLen / 2.0;

    //将坐标归一化到[-1,1]
    for (int i = 0; i < size; i++)
    {
        points.row(i) = points.row(i) / scale;
    }

    boundingBoxSideLength = boundingBoxSideLength / scale; //同时缩放boudingbox的大小（归一化boundingBox）
    Cube boundingBox(RowVector3d(0.0, 0.0, 0.0), boundingBoxSideLength[0], boundingBoxSideLength[1], boundingBoxSideLength[2]);
    boundingBox.positionSideLenToVerticesCuboid(); //将位置边长表达形式表示成顶点形式
    return boundingBox;
}

//点云转化为vector存储
vector<Point3d> PointCloud::pointsToVector()
{
    vector<Point3d> res(size);
    for (int i = 0; i < size; i++)
    {
        res[i] = Point3d(points.row(i));
    }
    return res;
}

//获取boundingBox
Cube PointCloud::getBoundingBox()
{
    RowVector3d max_xyz = points.colwise().maxCoeff(); //xyz坐标的最大值
    RowVector3d min_xyz = points.colwise().minCoeff(); //xyz坐标的最小值

    //包围盒中心
    RowVector3d boundingBoxCenter = (max_xyz + min_xyz) / 2.0;
    //boundingBox的xyz轴上的边长
    RowVector3d boundingBoxSideLength = max_xyz - min_xyz;

    Cube boundingBox(boundingBoxCenter, boundingBoxSideLength[0], boundingBoxSideLength[1], boundingBoxSideLength[2]);
    return boundingBox;
}

//体素化点云
set<Cube> PointCloud::voxelization(wh::basic::Cube &boundingBox, double leafSize)
{
    set<Cube> res;
    //先细分boundingBox，
    vector<Cube> voxel = boundingBox.subdivision(leafSize);

    //获取xyz方向细分的个数
    int xAmount = boundingBox.x / leafSize;
    int yAmount = boundingBox.y / leafSize;
    int zAmount = boundingBox.z / leafSize;

    // 边界位置增加一个cube
    // xAmount++;
    // yAmount++;
    // zAmount++;

    // 边界位置不增加cube，即一个点恰好在boundingBox的一个面上，不在增加细分cube的个数
    if (xAmount < boundingBox.x / leafSize)
    {
        xAmount++;
    }
    if (yAmount < boundingBox.y / leafSize)
    {
        yAmount++;
    }
    if (zAmount < boundingBox.z / leafSize)
    {
        zAmount++;
    }
        

    //原点
    RowVector3d origin = boundingBox.vertices.row(0);
    //体素的位置
    int xIndex = 0;
    int yIndex = 0;
    int zIndex = 0;

    for (int i = 0; i < points.rows(); i++)
    {
        RowVector3d index = (points.row(i) - origin) / leafSize; //获取体素位置
        xIndex = index[0];
        yIndex = index[1];
        zIndex = index[2];
        //计算体素在vector中的位置
        //边界位置处理（恰好在boundingBox的一个面上）
        if (xIndex == xAmount)
        {
            xIndex--;
        }
        if (yIndex == yAmount)
        {
            yIndex--;
        }
        if (zIndex == zAmount)
        {
            zIndex--;
        }
        //获取体素的位置
        int voxIndex = xIndex * yAmount * zAmount + yIndex * zAmount + zIndex;

        // index越界检验
        // if(voxel[voxIndex].vertices.rows()!=8){
        //     cout<<"index[0]:"<<index[0]<<endl;
        //     cout<<"index[1]:"<<index[1]<<endl;
        //     cout<<"index[2]:"<<index[2]<<endl;
        //     cout<<"xAmount:"<<xAmount<<" yAmount:"<<yAmount<<" zAmount:"<<zAmount<<endl;
        //     cout<<"xIndex:"<<xIndex<<" yIndex:"<<yIndex<<" zIndex:"<<zIndex<<endl;
        //     cout<<"index wrong:"<<voxIndex<<endl;
        // }
        res.insert(voxel[voxIndex]);
    }
    return res;
}

//获取C++原生数据
// float* PointCloud::getGLData()
// {
//     float pointCloudData[size * POINT3D_SIZE];
//     for(int i = 0; i < points.rows(); i++)
//     {
//         for(int j = 0; j < 3;j++)
//         {
//             pointCloudData[i + j] = points.row(i)[j];
//         }
//     }
//     return pointCloudData;
// }