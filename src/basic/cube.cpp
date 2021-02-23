#include <basic/cube.h>
using namespace std;
using namespace Eigen;
using namespace wh::basic;

//构造函数
Cube::Cube() : position(0.0, 0.0, 0.0), sideLen(0.0), vertices(), x(0.0), y(0.0), z(0.0) {}

//用所有的点表示cube
Cube::Cube(MatrixXd vertices) : vertices(vertices)
{
    RowVector3d center = vertices.colwise().sum(); //colwise()按照矩阵每一列的方向上排列 这里相当于每一行相加
    position = center / vertices.rows();

    RowVector3d temp = vertices.row(1) - vertices.row(0);
    x = sqrt(temp.dot(temp));

    temp = vertices.row(3) - vertices.row(0);
    y = sqrt(temp.dot(temp));

    temp = vertices.row(7) - vertices.row(0);
    z = sqrt(temp.dot(temp));

    if ((x - y < 0.00000001) && (y - z < 0.00000001))
    {
        sideLen = x;
    }
}

//中心位置和边长表示Cube
Cube::Cube(RowVector3d position, double sideLen) : position(position), sideLen(sideLen)
{
    double halfSize = sideLen / 2.0;
    vertices.resize(8, 3);

    vertices.row(0) = RowVector3d(position[0] - halfSize, position[1] - halfSize, position[2] - halfSize);
    vertices.row(1) = RowVector3d(position[0] + halfSize, position[1] - halfSize, position[2] - halfSize);
    vertices.row(2) = RowVector3d(position[0] + halfSize, position[1] + halfSize, position[2] - halfSize);
    vertices.row(3) = RowVector3d(position[0] - halfSize, position[1] + halfSize, position[2] - halfSize);
    vertices.row(4) = RowVector3d(position[0] - halfSize, position[1] + halfSize, position[2] + halfSize);
    vertices.row(5) = RowVector3d(position[0] + halfSize, position[1] + halfSize, position[2] + halfSize);
    vertices.row(6) = RowVector3d(position[0] + halfSize, position[1] - halfSize, position[2] + halfSize);
    vertices.row(7) = RowVector3d(position[0] - halfSize, position[1] - halfSize, position[2] + halfSize);
    // positionSideLenToVertices();

    x = sideLen;
    y = sideLen;
    z = sideLen;
}

//长方体
Cube::Cube(Eigen::RowVector3d position, double x, double y, double z) : position(position), x(x), y(y), z(z)
{
    sideLen = 0.0;
    positionSideLenToVerticesCuboid();
}

//运算符重载
bool Cube::operator<(const Cube &cube) const
{
    if (position[0] < cube.position[0])
    {
        return true;
    }
    if (position[1] < cube.position[1])
    {
        return true;
    }
    if (position[2] < cube.position[2])
    {
        return true;
    }
    return false;
}

//显示信息
void Cube::showInf()
{
    cout << "position:" << position << endl;
    cout << "sideLen:" << sideLen << endl;
    cout << "x:" << x << " y:" << y << " z:" << z << endl;
}

//显示信息
void Cube::showInf() const
{
    cout << "position:" << position << endl;
    cout << "sideLen:" << sideLen << endl;
    cout << "x:" << x << " y:" << y << " z:" << z << endl;
}

//将position sideLen转化成顶点表达式
void Cube::positionSideLenToVertices()
{
    vertices.resize(8, 3);
    double halfSize = sideLen / 2.0;
    vertices.row(0) = RowVector3d(position[0] - halfSize, position[1] - halfSize, position[2] - halfSize);
    vertices.row(1) = RowVector3d(position[0] + halfSize, position[1] - halfSize, position[2] - halfSize);
    vertices.row(2) = RowVector3d(position[0] + halfSize, position[1] + halfSize, position[2] - halfSize);
    vertices.row(3) = RowVector3d(position[0] - halfSize, position[1] + halfSize, position[2] - halfSize);
    vertices.row(4) = RowVector3d(position[0] - halfSize, position[1] + halfSize, position[2] + halfSize);
    vertices.row(5) = RowVector3d(position[0] + halfSize, position[1] + halfSize, position[2] + halfSize);
    vertices.row(6) = RowVector3d(position[0] + halfSize, position[1] - halfSize, position[2] + halfSize);
    vertices.row(7) = RowVector3d(position[0] - halfSize, position[1] - halfSize, position[2] + halfSize);
}

//将position sideLen转化成顶点表达式
void Cube::positionSideLenToVerticesCuboid()
{
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

//细分
//TODO
//参数：细分的总个数
vector<Cube> Cube::subdivision(int amount)
{
    vector<Cube> res;
    return res;
}

//细分
//细分的立方体边长
vector<Cube> Cube::subdivision(double leafSize)
{
    vector<Cube> res;
    //Cube的边长不够细分
    if (x <= leafSize || y <= leafSize || z <= leafSize)
        return res;

    //获取xyz方向细分的个数
    int xAmount = x / leafSize;
    int yAmount = y / leafSize;
    int zAmount = z / leafSize;

    // 边界位置再增加一个cube
    // xAmount++;
    // yAmount++;
    // zAmount++;

    //边界位置不增加cube的数量
    if (xAmount < x / leafSize)
    {
        xAmount++;
    }
    if (yAmount < y / leafSize)
    {
        yAmount++;
    }
    if (zAmount < z / leafSize)
    {
        zAmount++;
    }
        
    double halfSize = leafSize / 2.0;

    //计算细分后的正方体
    for (int i = 0; i < xAmount; i++)
    {
        for (int j = 0; j < yAmount; j++)
        {
            for (int k = 0; k < zAmount; k++)
            {
                RowVector3d pos((2 * i + 1) * halfSize, (2 * j + 1) * halfSize, (2 * k + 1) * halfSize);
                pos += vertices.row(0);
                res.push_back(Cube(pos, leafSize));//按照xyz轴的顺序优先存放体素
            }
        }
    }
    return res;
}

// 获取Cube中心
Eigen::RowVector3d Cube::getCenter()
{
    return position;
}

// 获取最大边长
double Cube::getMaxSideLen()
{
    double res = (x > y) ? x : y;
    return (res > z) ? res : z;
}