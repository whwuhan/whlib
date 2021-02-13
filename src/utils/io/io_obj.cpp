#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <utils/io/io_obj.h>
#include <utils/string_lib.h>
#include <basic/polygon_mesh.h>
using namespace std;
using namespace wh::utils::io;
using namespace wh::basic;
//读取obj点云文件
void wh::utils::io::loadPointCloudObj(const string fileName, struct PointCloud *pointCloudPtr)
{
    //打开文件
    ifstream dataSrc(fileName);
    if (!dataSrc.is_open())
    {
        cout << "no data source." << endl;
        return;
    }

    //读入文件
    string line;
    vector<string> lineSplit;
    unsigned int count = 0; //obj文件点的数量

    while (getline(dataSrc, line))
    {
        if (line[0] == 'v')
        {
            //lineSplit = split(line, " ");
            count++;
            //cout << count << endl;
            /** resize()效率太低，改成先遍历点的个数，再resize()一次
            pointCloudPtr->conservativeResize((++pointCloudPtr->size), 3);
            pointCloudPtr->points(pointCloudPtr->size-1, 0) = atof(lineSplit[1].c_str());
            pointCloudPtr->points(pointCloudPtr->size-1, 1) = atof(lineSplit[2].c_str());
            pointCloudPtr->points(pointCloudPtr->size-1, 2) = atof(lineSplit[3].c_str());
            */
        }
    }
    cout << "Point Cloud size:" << count << endl;
    dataSrc.clear(); //先要clear()才能回到文件头
    dataSrc.seekg(0, ios::beg);
    pointCloudPtr->resize(count, 3);
    count = 0;

    while (getline(dataSrc, line))
    {
        switch (line[0])
        {
        case 'v':
            lineSplit = split(line, " ");
            pointCloudPtr->points(count, 0) = atof(lineSplit[1].c_str());
            pointCloudPtr->points(count, 1) = atof(lineSplit[2].c_str());
            pointCloudPtr->points(count, 2) = atof(lineSplit[3].c_str());
            count++;
            break;
        }
    }
    cout << "Load Point Cloud Successfully!" << endl;
    dataSrc.close();
}

//点云存入obj文件
void wh::utils::io::savePointCloudObj(const string fileName, const struct PointCloud *const pointCloudPtr)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib point cloud obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入数据
    for (int i = 0; i < pointCloudPtr->size; i++)
    {
        dataDes << "v"
                         << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[0];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[1];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[2] << endl;
    }
    cout << "Save Point Cloud Successfully!" << endl;
    dataDes.close();
}

//cube triangular mesh 写入obj文件
void wh::utils::io::saveTriCubeMeshObj(const std::string fileName, wh::basic::Cube *cubePtr)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cubePtr->vertices.rows(); i++)
    {
        dataDes << "v"
                         << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[0];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[1];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[2] << endl;
    }

    //写入面数据（注意顺序首个索引从小到大）
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "2"
                     << " "
                     << "3" << endl;
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "3"
                     << " "
                     << "4" << endl;
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "4"
                     << " "
                     << "5" << endl;
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "5"
                     << " "
                     << "8" << endl;
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "2"
                     << " "
                     << "7" << endl;
    dataDes << "f"
                     << " "
                     << "1"
                     << " "
                     << "7"
                     << " "
                     << "8" << endl;
    dataDes << "f"
                     << " "
                     << "2"
                     << " "
                     << "3"
                     << " "
                     << "6" << endl;
    dataDes << "f"
                     << " "
                     << "2"
                     << " "
                     << "6"
                     << " "
                     << "7" << endl;
    dataDes << "f"
                     << " "
                     << "3"
                     << " "
                     << "4"
                     << " "
                     << "6" << endl;
    dataDes << "f"
                     << " "
                     << "4"
                     << " "
                     << "5"
                     << " "
                     << "6" << endl;
    dataDes << "f"
                     << " "
                     << "5"
                     << " "
                     << "6"
                     << " "
                     << "7" << endl;
    dataDes << "f"
                     << " "
                     << "5"
                     << " "
                     << "7"
                     << " "
                     << "8" << endl;
    dataDes.close();
}

//保存triangular cube meshes
void wh::utils::io::saveTriCubeMeshesObj(const std::string fileName, const std::vector<wh::basic::Cube> &cubes)
{
    //打开文件
    ofstream dataDes(fileName);

    dataDes << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        //cout<<cube.vertices.size()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            dataDes << "v"
                             << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //存入面数据
    for (int i = 0; i < cubes.size(); i++)
    {
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 5 << " " << 8 * i + 8 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        dataDes << "f"
                         << " " << 8 * i + 2 << " " << 8 * i + 3 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 2 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 3 << " " << 8 * i + 4 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 4 << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 5 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 5 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
    }

    dataDes.close();
}

//重载保存triangular cube meshes
void wh::utils::io::saveTriCubeMeshesObj(const std::string fileName, const std::set<wh::basic::Cube> &cubes)
{
    //打开文件
    ofstream dataDes(fileName);

    dataDes << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        // cout<<cube.vertices.rows()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            dataDes << "v"
                             << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //存入面数据
    // cout<<"size:"<<cubes.size()<<endl;
    for (int i = 0; i < cubes.size(); i++)
    {
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 5 << " " << 8 * i + 8 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 1 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        dataDes << "f"
                         << " " << 8 * i + 2 << " " << 8 * i + 3 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 2 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 3 << " " << 8 * i + 4 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 4 << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        dataDes << "f"
                         << " " << 8 * i + 5 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "f"
                         << " " << 8 * i + 5 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
    }

    dataDes.close();
}

//cube wireframe（线框写入obj文件）
void wh::utils::io::saveCubeWireframeObj(const std::string fileName, wh::basic::Cube *cubePtr)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib cube wireframe obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cubePtr->vertices.rows(); i++)
    {
        dataDes << "v"
                         << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[0];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[1];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[2] << endl;
    }

    //写入线框（注意顺序首个索引从小到大）
    dataDes << "l"
                     << " "
                     << "1"
                     << " "
                     << "2" << endl;
    dataDes << "l"
                     << " "
                     << "2"
                     << " "
                     << "3" << endl;
    dataDes << "l"
                     << " "
                     << "3"
                     << " "
                     << "4" << endl;
    dataDes << "l"
                     << " "
                     << "4"
                     << " "
                     << "1" << endl;
    dataDes << "l"
                     << " "
                     << "4"
                     << " "
                     << "5" << endl;
    dataDes << "l"
                     << " "
                     << "5"
                     << " "
                     << "6" << endl;
    dataDes << "l"
                     << " "
                     << "6"
                     << " "
                     << "3" << endl;
    dataDes << "l"
                     << " "
                     << "6"
                     << " "
                     << "7" << endl;
    dataDes << "l"
                     << " "
                     << "7"
                     << " "
                     << "2" << endl;
    dataDes << "l"
                     << " "
                     << "7"
                     << " "
                     << "8" << endl;
    dataDes << "l"
                     << " "
                     << "8"
                     << " "
                     << "1" << endl;
    dataDes << "l"
                     << " "
                     << "8"
                     << " "
                     << "5" << endl;
    dataDes.close();
}

//cube wireframes（多个线框写入obj文件）
void wh::utils::io::saveCubeWireframesObj(const string fileName, const vector<wh::basic::Cube> &cubes)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            dataDes << "v"
                             << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++)
    {
        dataDes << "l"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << endl;
        dataDes << "l"
                         << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        dataDes << "l"
                         << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        dataDes << "l"
                         << " " << 8 * i + 4 << " " << 8 * i + 1 << endl;
        dataDes << "l"
                         << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        dataDes << "l"
                         << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        dataDes << "l"
                         << " " << 8 * i + 6 << " " << 8 * i + 3 << endl;
        dataDes << "l"
                         << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "l"
                         << " " << 8 * i + 7 << " " << 8 * i + 2 << endl;
        dataDes << "l"
                         << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        dataDes << "l"
                         << " " << 8 * i + 8 << " " << 8 * i + 1 << endl;
        dataDes << "l"
                         << " " << 8 * i + 8 << " " << 8 * i + 5 << endl;
    }
    dataDes.close();
}

void wh::utils::io::saveCubeWireframesObj(const std::string fileName, const std::set<wh::basic::Cube> &cubes)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            dataDes << "v"
                             << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++)
    {
        dataDes << "l"
                         << " " << 8 * i + 1 << " " << 8 * i + 2 << endl;
        dataDes << "l"
                         << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        dataDes << "l"
                         << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        dataDes << "l"
                         << " " << 8 * i + 4 << " " << 8 * i + 1 << endl;
        dataDes << "l"
                         << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        dataDes << "l"
                         << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        dataDes << "l"
                         << " " << 8 * i + 6 << " " << 8 * i + 3 << endl;
        dataDes << "l"
                         << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        dataDes << "l"
                         << " " << 8 * i + 7 << " " << 8 * i + 2 << endl;
        dataDes << "l"
                         << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        dataDes << "l"
                         << " " << 8 * i + 8 << " " << 8 * i + 1 << endl;
        dataDes << "l"
                         << " " << 8 * i + 8 << " " << 8 * i + 5 << endl;
    }
    dataDes.close();
}

//读取polygon mesh
void wh::utils::io::loadPolygonMeshObj(const std::string fileName, wh::basic::PolygonMesh *polygonMeshPtr)
{
    //打开文件
    ifstream dataSrc(fileName);
    if (!dataSrc.is_open())
    {
        cout << "no data source." << endl;
        return;
    }

    //读入文件
    string line;
    vector<string> lineSplit;
    unsigned int verticesAmount = 0; //顶点数量
    unsigned int facesAmount = 0;    //面片数量

    bool polygonMeshTypeIsConfirmed = false; // polygon mesh类型是否确定
    int polygonType = 3;                        //polygon mesh的类型（三角面片还是四边形面片，默认是三角面片）
    //确定
    while (getline(dataSrc, line))
    {
        if (line[0] == 'v')
        {
            //lineSplit = split(line, " ");
            verticesAmount++;
            //cout << count << endl;
            /** resize()效率太低，改成先遍历点的个数，再resize()一次
            pointCloudPtr->conservativeResize((++pointCloudPtr->size), 3);
            pointCloudPtr->points(pointCloudPtr->size-1, 0) = atof(lineSplit[1].c_str());
            pointCloudPtr->points(pointCloudPtr->size-1, 1) = atof(lineSplit[2].c_str());
            pointCloudPtr->points(pointCloudPtr->size-1, 2) = atof(lineSplit[3].c_str());
            */
        }

        if (line[0] == 'f')
        {
            facesAmount++;
            if (!polygonMeshTypeIsConfirmed)
            {
                lineSplit = split(line, " ");
                polygonType = lineSplit.size() - 1;
            }
        }
    }

    dataSrc.clear(); //先要clear()才能回到文件头
    dataSrc.seekg(0, ios::beg);

    polygonMeshPtr->vertices.resize(verticesAmount, 3);
    polygonMeshPtr->faces.resize(facesAmount, polygonType);

    cout << "verticesAmount:" << verticesAmount << endl;
    cout << "facesAmount:" << facesAmount << endl;

    verticesAmount = 0;
    facesAmount = 0;
    while (getline(dataSrc, line))
    {
        switch (line[0])
        {
        case 'v':
            lineSplit = split(line, " ");
            polygonMeshPtr->vertices(verticesAmount, 0) = atof(lineSplit[1].c_str());
            polygonMeshPtr->vertices(verticesAmount, 1) = atof(lineSplit[2].c_str());
            polygonMeshPtr->vertices(verticesAmount, 2) = atof(lineSplit[3].c_str());
            verticesAmount++;
            break;
        case 'f':
            lineSplit = split(line, " ");
            for (int i = 0; i < polygonType; i++)
            {
                polygonMeshPtr->faces(facesAmount, i) = atoi(lineSplit[i + 1].c_str());
            }
            facesAmount++;
            break;
        default:
            cout << line << endl;
        }
    }
    cout << "Load Polygon Mesh Successfull!" << endl;
    dataSrc.close();
}

//保存Polygon mesh
void wh::utils::io::savePolygonMeshObj(const std::string fileName, wh::basic::PolygonMesh *polygonMeshPtr)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib polygon mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象
    cout << "verticesAmount:" << polygonMeshPtr->vertices.rows() << endl;
    cout << "facesAmount:" << polygonMeshPtr->faces.rows() << endl;
    //存入点数据
    for (int i = 0; i < polygonMeshPtr->vertices.rows(); i++)
    {
        dataDes << "v"
                         << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[0];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[1];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[2] << endl;
    }

    //写入面片信息
    int polygonMeshType = polygonMeshPtr->faces.cols(); //获取是三角面片是是正方形面片
    for (int i = 0; i < polygonMeshPtr->faces.rows(); i++)
    {
        dataDes << "f";
        for (int j = 0; j < polygonMeshType; j++)
        {
            //cout<<"face:"<<polygonMeshPtr->faces.row(i)[j]<<endl;
            dataDes << " " << polygonMeshPtr->faces.row(i)[j];
        }
        dataDes << endl;
    }
    cout << "Save Polygon Mesh Successfully!" << endl;
    dataDes.close();
}

// load skeleton obj
void wh::utils::io::loadSkeletonObj(const std::string fileName, wh::basic::Skeleton *skeletonPtr)
{
    //打开文件
    ifstream dataSrc(fileName);
    if (!dataSrc.is_open())
    {
        cout << "no data source." << endl;
        return;
    }

    //读入文件
    string line;
    vector<string> lineSplit;
    unsigned int pointsAmount = 0; //骨架点数量
    unsigned int edgesAmount = 0;  //连线数量

    //确定数量
    while (getline(dataSrc, line))
    {
        if (line[0] == 'v')
        {
            pointsAmount++;
        }
        if (line[0] == 'l')
        {
            edgesAmount++;
        }
    }

    dataSrc.clear(); //先要clear()才能回到文件头
    dataSrc.seekg(0, ios::beg);

    cout << "pointsAmount:" << pointsAmount << endl;
    cout << "edgesAmount:" << edgesAmount << endl;

    skeletonPtr->points.resize(pointsAmount, 3);
    skeletonPtr->edges.resize(edgesAmount, 2);

    pointsAmount = 0;
    edgesAmount = 0;
    while (getline(dataSrc, line))
    {
        switch (line[0])
        {
        case 'v':
            lineSplit = split(line, " ");
            skeletonPtr->points(pointsAmount, 0) = atof(lineSplit[1].c_str());
            skeletonPtr->points(pointsAmount, 1) = atof(lineSplit[2].c_str());
            skeletonPtr->points(pointsAmount, 2) = atof(lineSplit[3].c_str());
            pointsAmount++;
            break;
        case 'l':
            lineSplit = split(line, " ");
            skeletonPtr->edges(edgesAmount, 0) = atoi(lineSplit[1].c_str());
            skeletonPtr->edges(edgesAmount, 1) = atoi(lineSplit[2].c_str());
            edgesAmount++;
            break;
        default:
            cout << line << endl;
        }
    }
    cout << "Load Skeleton Successfully!" << endl;
    dataSrc.close();
}

// save skeleton obj
void wh::utils::io::saveSkeletonObj(const std::string fileName, wh::basic::Skeleton *skeletonPtr)
{
    //打开文件
    ofstream dataDes(fileName);
    dataDes << "# whlib Skeleton obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    dataDes << "# " << dateTime; //写入存储时间

    //存储对象名
    std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    dataDes << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    int pointsSize = skeletonPtr->points.rows();
    for (int i = 0; i < pointsSize; i++)
    { //写入骨架点
        dataDes << "v"
                         << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[0];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[1];
        dataDes << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[2] << endl;
    }

    int edgesSize = skeletonPtr->edges.rows();
    for (int i = 0; i < edgesSize; i++)
    {
        dataDes << "l"
                         << " " << skeletonPtr->edges(i, 0);
        dataDes << " " << skeletonPtr->edges(i, 1) << endl;
    }
    cout << "Save Skeleton Successfully!" << endl;
    dataDes.close();
}