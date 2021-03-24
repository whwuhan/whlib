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
        lineSplit = split(line, " ");
        if(lineSplit.size() != 0)
        {
            if(lineSplit[0] == "v")
            {
                count++;
            }
        }
    }
    cout << "Point Cloud size:" << count << endl;
    dataSrc.clear(); //先要clear()才能回到文件头
    dataSrc.seekg(0, ios::beg);
    pointCloudPtr->resize(count, 3);
    count = 0;

    while (getline(dataSrc, line))
    {
        lineSplit = split(line, " ");
        if(lineSplit.size() != 0)//防止空行
        {
            if(lineSplit[0] == "v")
            {
                pointCloudPtr->points(count, 0) = stof(lineSplit[1]);
                pointCloudPtr->points(count, 1) = stof(lineSplit[2]);
                pointCloudPtr->points(count, 2) = stof(lineSplit[3]);
                count++;
            }
        }
    }
    cout << "Load Point Cloud Successfully!" << endl;
    dataSrc.close();
}
        
//点云存入obj文件
void wh::utils::io::savePointCloudObj(const string fileName, const struct PointCloud *const pointCloudPtr)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib point cloud obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入数据
    for (int i = 0; i < pointCloudPtr->size; i++)
    {
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << pointCloudPtr->points.row(i)[2] << endl;
    }
    cout << "Save Point Cloud Successfully!" << endl;
    data_des.close();
}

//cube triangular mesh 写入obj文件
void wh::utils::io::saveTriCubeMeshObj(const string fileName, Cube *cubePtr)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cubePtr->vertices.rows(); i++)
    {
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[2] << endl;
    }

    //写入面数据（注意顺序首个索引从小到大）
    data_des << "f" << " " << "1" << " " << "2" << " " << "3" << endl;
    data_des << "f" << " " << "1" << " " << "3" << " " << "4" << endl;
    data_des << "f" << " " << "1" << " " << "4" << " " << "5" << endl;
    data_des << "f" << " " << "1" << " " << "5" << " " << "8" << endl;
    data_des << "f" << " " << "1" << " " << "2" << " " << "7" << endl;
    data_des << "f" << " " << "1" << " " << "7" << " " << "8" << endl;
    data_des << "f" << " " << "2" << " " << "3" << " " << "6" << endl;
    data_des << "f" << " " << "2" << " " << "6" << " " << "7" << endl;
    data_des << "f" << " " << "3" << " " << "4" << " " << "6" << endl;
    data_des << "f" << " " << "4" << " " << "5" << " " << "6" << endl;
    data_des << "f" << " " << "5" << " " << "6" << " " << "7" << endl;
    data_des << "f" << " " << "5" << " " << "7" << " " << "8" << endl;
    cout << "Save Triangular Cube Mesh Successfully!" << endl;
    data_des.close();
}

//保存triangular cube meshes
void wh::utils::io::saveTriCubeMeshesObj(const string fileName, const vector<Cube> &cubes)
{
    //打开文件
    ofstream data_des(fileName);

    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        //cout<<cube.vertices.size()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //存入面数据
    for (int i = 0; i < cubes.size(); i++)
    {
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 5 << " " << 8 * i + 8 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        data_des << "f" << " " << 8 * i + 2 << " " << 8 * i + 3 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 2 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 3 << " " << 8 * i + 4 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 4 << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 5 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 5 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
    }
    cout << "Save Triangular Cube Meshes Successfully!" << endl;
    data_des.close();
}

//重载保存triangular cube meshes
void wh::utils::io::saveTriCubeMeshesObj(const string fileName, const set<Cube> &cubes)
{
    //打开文件
    ofstream data_des(fileName);

    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes){
        // cout<<cube.vertices.rows()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++){
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //法线占位
    for (Cube cube : cubes){
        // cout<<cube.vertices.rows()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++){
            data_des << "vn" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //存入面数据
    // cout<<"size:"<<cubes.size()<<endl;
    for (int i = 0; i < cubes.size(); i++){
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 5 << " " << 8 * i + 8 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 2 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 1 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        data_des << "f" << " " << 8 * i + 2 << " " << 8 * i + 3 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 2 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 3 << " " << 8 * i + 4 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 4 << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        data_des << "f" << " " << 8 * i + 5 << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "f" << " " << 8 * i + 5 << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
    }
    cout << "Save Triangular Cube Meshes Successfully!" << endl;
    data_des.close();
}

//cube wireframe（线框写入obj文件）
void wh::utils::io::saveCubeWireframeObj(const string fileName, Cube *cubePtr)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib cube wireframe obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cubePtr->vertices.rows(); i++)
    {
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cubePtr->vertices.row(i)[2] << endl;
    }

    //写入线框（注意顺序首个索引从小到大）
    data_des << "l" << " " << "1" << " " << "2" << endl;
    data_des << "l" << " " << "2" << " " << "3" << endl;
    data_des << "l" << " " << "3" << " " << "4" << endl;
    data_des << "l" << " " << "4" << " " << "1" << endl;
    data_des << "l" << " " << "4" << " " << "5" << endl;
    data_des << "l" << " " << "5" << " " << "6" << endl;
    data_des << "l" << " " << "6" << " " << "3" << endl;
    data_des << "l" << " " << "6" << " " << "7" << endl;
    data_des << "l" << " " << "7" << " " << "2" << endl;
    data_des << "l" << " " << "7" << " " << "8" << endl;
    data_des << "l" << " " << "8" << " " << "1" << endl;
    data_des << "l" << " " << "8" << " " << "5" << endl;
    cout << "Save Triangular Cube Wireframe Successfully!" << endl;
    data_des.close();
}

//cube wireframes（多个线框写入obj文件）
void wh::utils::io::saveCubeWireframesObj(const string fileName, const vector<Cube> &cubes)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++)
    {
        data_des << "l" << " " << 8 * i + 1 << " " << 8 * i + 2 << endl;
        data_des << "l" << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        data_des << "l" << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        data_des << "l" << " " << 8 * i + 4 << " " << 8 * i + 1 << endl;
        data_des << "l" << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        data_des << "l" << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        data_des << "l" << " " << 8 * i + 6 << " " << 8 * i + 3 << endl;
        data_des << "l" << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "l" << " " << 8 * i + 7 << " " << 8 * i + 2 << endl;
        data_des << "l" << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        data_des << "l" << " " << 8 * i + 8 << " " << 8 * i + 1 << endl;
        data_des << "l" << " " << 8 * i + 8 << " " << 8 * i + 5 << endl;
    }
    cout << "Save Triangular Cube Wireframes Successfully!" << endl;
    data_des.close();
}

void wh::utils::io::saveCubeWireframesObj(const string fileName, const set<Cube> &cubes)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes)
    {
        for (int i = 0; i < cube.vertices.rows(); i++)
        {
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++)
    {
        data_des << "l" << " " << 8 * i + 1 << " " << 8 * i + 2 << endl;
        data_des << "l" << " " << 8 * i + 2 << " " << 8 * i + 3 << endl;
        data_des << "l" << " " << 8 * i + 3 << " " << 8 * i + 4 << endl;
        data_des << "l" << " " << 8 * i + 4 << " " << 8 * i + 1 << endl;
        data_des << "l" << " " << 8 * i + 4 << " " << 8 * i + 5 << endl;
        data_des << "l" << " " << 8 * i + 5 << " " << 8 * i + 6 << endl;
        data_des << "l" << " " << 8 * i + 6 << " " << 8 * i + 3 << endl;
        data_des << "l" << " " << 8 * i + 6 << " " << 8 * i + 7 << endl;
        data_des << "l" << " " << 8 * i + 7 << " " << 8 * i + 2 << endl;
        data_des << "l" << " " << 8 * i + 7 << " " << 8 * i + 8 << endl;
        data_des << "l" << " " << 8 * i + 8 << " " << 8 * i + 1 << endl;
        data_des << "l" << " " << 8 * i + 8 << " " << 8 * i + 5 << endl;
    }
    cout << "Save Triangular Cube Wireframes Successfully!" << endl;
    data_des.close();
}

//读取polygon mesh
void wh::utils::io::loadPolygonMeshObj(const string fileName, PolygonMesh *polygonMeshPtr)
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
    unsigned int facesAmount = 0;  //面片数量
    unsigned int UVAmount = 0;//纹理坐标数量
    unsigned int normalsAmount = 0;//法线数量

    int polygonType = 3;                     //polygon mesh的类型（三角面片还是四边形面片，默认是三角面片）

    //获取数量
    while (getline(dataSrc, line))
    {
        lineSplit = split(line, " ");
        if(lineSplit.size() != 0)
        {
            if(lineSplit[0] == "v") verticesAmount++;
            if(lineSplit[0] == "vt") UVAmount++;
            if(lineSplit[0] == "vn") normalsAmount++;
            if(lineSplit[0] == "f") facesAmount++;
        }
    }

    dataSrc.clear(); //先要clear()才能回到文件头
    dataSrc.seekg(0, ios::beg);
    //注意 uv坐标 normal 的数量可能不等于面片的数量
    //分配空间
    polygonMeshPtr->vertices.resize(verticesAmount, 3);
    polygonMeshPtr->verticesIndices.resize(facesAmount, polygonType);
    polygonMeshPtr->UVs.resize(UVAmount, 2);
    polygonMeshPtr->UVIndices.resize(facesAmount, polygonType);
    polygonMeshPtr->normals.resize(normalsAmount, 3);
    polygonMeshPtr->normalsIndices.resize(facesAmount, polygonType);
    

    cout << "verticesAmount:" << verticesAmount << endl;
    cout << "facesAmount:" << facesAmount << endl;
    cout << "UVAmount:" << UVAmount << endl;
    cout << "normalsAmount:" << normalsAmount << endl;

    verticesAmount = 0;
    facesAmount = 0;
    UVAmount = 0;
    normalsAmount = 0;
    
    //读取数据
    while (getline(dataSrc, line))
    {
        lineSplit = split(line, " ");
        if(lineSplit.size() != 0)
        {
            //顶点
            if(lineSplit[0] == "v")
            {
                polygonMeshPtr->vertices(verticesAmount, 0) = stof(lineSplit[1]);
                polygonMeshPtr->vertices(verticesAmount, 1) = stof(lineSplit[2]);
                polygonMeshPtr->vertices(verticesAmount, 2) = stof(lineSplit[3]);
                verticesAmount++;
            }
            //顶点UV坐标
            if(lineSplit[0] == "vt")
            {
                polygonMeshPtr->UVs(UVAmount, 0) = stof(lineSplit[1]);
                polygonMeshPtr->UVs(UVAmount, 1) = stof(lineSplit[2]);
                UVAmount++;
            }
            //顶点法线
            if(lineSplit[0] == "vn")
            {
                polygonMeshPtr->normals(normalsAmount, 0) = stof(lineSplit[1]);
                polygonMeshPtr->normals(normalsAmount, 1) = stof(lineSplit[2]);
                polygonMeshPtr->normals(normalsAmount, 2) = stof(lineSplit[3]);
                normalsAmount++;
            }
            //面片包含 vertex/uv/normals
            if(lineSplit[0] == "f" && split(lineSplit[1], "/").size() == 3 )
            {   
                vector<string> indicesSplit1 = split(lineSplit[1], "/");
                vector<string> indicesSplit2 = split(lineSplit[2], "/");
                vector<string> indicesSplit3 = split(lineSplit[3], "/");
                //面片顶点索引
                polygonMeshPtr->verticesIndices(facesAmount, 0) = stoi(indicesSplit1[0]);
                polygonMeshPtr->verticesIndices(facesAmount, 1) = stoi(indicesSplit2[0]);
                polygonMeshPtr->verticesIndices(facesAmount, 2) = stoi(indicesSplit3[0]);
                //面片UV坐标索引
                polygonMeshPtr->UVIndices(facesAmount, 0) = stoi(indicesSplit1[1]);
                polygonMeshPtr->UVIndices(facesAmount, 1) = stoi(indicesSplit2[1]);
                polygonMeshPtr->UVIndices(facesAmount, 2) = stoi(indicesSplit3[1]);
                //面片顶点法线索引
                polygonMeshPtr->normalsIndices(facesAmount, 0) = stoi(indicesSplit1[2]);
                polygonMeshPtr->normalsIndices(facesAmount, 1) = stoi(indicesSplit2[2]);
                polygonMeshPtr->normalsIndices(facesAmount, 2) = stoi(indicesSplit3[2]);
                facesAmount++;
            }
            else if(lineSplit[0] == "f")//面片只包含vertex index
            {
                polygonMeshPtr->verticesIndices(facesAmount, 0) = stoi(lineSplit[1]);
                polygonMeshPtr->verticesIndices(facesAmount, 1) = stoi(lineSplit[2]);
                polygonMeshPtr->verticesIndices(facesAmount, 2) = stoi(lineSplit[3]);
            }
        }
    }
    // cout << polygonMeshPtr->verticesIndices.rows() << endl;
    // cout << polygonMeshPtr->UVIndices.rows() << endl;
    // cout << polygonMeshPtr->normalsIndices.rows() << endl;
    // cout << polygonMeshPtr->vertices.rows() << endl;
    // cout << polygonMeshPtr->UVs.rows() << endl;
    // cout << polygonMeshPtr->normals.rows() << endl;
    cout << "Load Polygon Mesh Successfully!" << endl;
    dataSrc.close();
}

//保存Polygon mesh
void wh::utils::io::savePolygonMeshObj(const string fileName, PolygonMesh *polygonMeshPtr)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib polygon mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象
    cout << "verticesAmount:" << polygonMeshPtr->vertices.rows() << endl;
    cout << "facesAmount:" << polygonMeshPtr->verticesIndices.rows() << endl;
    //存入点数据
    for (int i = 0; i < polygonMeshPtr->vertices.rows(); i++)
    {
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << polygonMeshPtr->vertices.row(i)[2] << endl;
    }
    //写入面片信息
    int polygonMeshType = polygonMeshPtr->verticesIndices.cols(); //获取是三角面片是是正方形面片
    for (int i = 0; i < polygonMeshPtr->verticesIndices.rows(); i++)
    {
        data_des << "f";
        for (int j = 0; j < polygonMeshType; j++)
        {
            //cout<<"face:"<<polygonMeshPtr->verticesIndices.row(i)[j]<<endl;
            data_des << " " << polygonMeshPtr->verticesIndices.row(i)[j];
        }
        data_des << endl;
    }
    cout << "Save Polygon Mesh Successfully!" << endl;
    data_des.close();
}

// load skeleton obj
void wh::utils::io::loadSkeletonObj(const string fileName, Skeleton *skeletonPtr)
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
void wh::utils::io::saveSkeletonObj(const string fileName, Skeleton *skeletonPtr)
{
    //打开文件
    ofstream data_des(fileName);
    data_des << "# whlib Skeleton obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string dateTime(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << dateTime; //写入存储时间

    //存储对象名
    vector<string> fileNameSplit = wh::utils::split(fileName, "/.\\");
    int fileNameIndex = fileNameSplit.size() - 2;
    data_des << "o " << fileNameSplit[fileNameIndex] << endl; //obj对象

    int pointsSize = skeletonPtr->points.rows();
    for (int i = 0; i < pointsSize; i++)
    { //写入骨架点
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << skeletonPtr->points.row(i)[2] << endl;
    }

    int edgesSize = skeletonPtr->edges.rows();
    for (int i = 0; i < edgesSize; i++)
    {
        data_des << "l" << " " << skeletonPtr->edges(i, 0);
        data_des << " " << skeletonPtr->edges(i, 1) << endl;
    }
    cout << "Save Skeleton Successfully!" << endl;
    data_des.close();
}