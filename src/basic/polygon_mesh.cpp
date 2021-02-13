#include <basic/polygon_mesh.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <basic/edge.h>
#include <basic/point3d.h>
#include <basic/face.h>
using namespace std;
using namespace wh::basic;
using namespace Eigen;
bool is_exist(const unsigned int i, const RowVector3i &vec); //判断vec中是否存在i
//构造函数
wh::basic::PolygonMesh::PolygonMesh() : vertices(), faces()
{
}

wh::basic::PolygonMesh::PolygonMesh(Eigen::MatrixXd &vertices, Eigen::MatrixXi &faces) : vertices(vertices), faces(faces)
{
}

set<Edge> PolygonMesh::creatUnorderedEdges()
{
    set<Edge> res;
    for (int i = 0; i < faces.rows(); i++)
    {
        res.insert(Edge(faces(i, 0), faces(i, 1)));
        res.insert(Edge(faces(i, 1), faces(i, 2)));
        res.insert(Edge(faces(i, 2), faces(i, 0)));
    }
    return res;
}

std::set<wh::basic::Edge> PolygonMesh::creatOrderedEdges()
{
    set<Edge> res;
    for (int i = 0; i < faces.rows(); i++)
    {
        res.insert(Edge(faces(i, 0), faces(i, 1), false));
        res.insert(Edge(faces(i, 1), faces(i, 2), false));
        res.insert(Edge(faces(i, 2), faces(i, 0), false));
    }
    return res;
}

//普通三角细分
void wh::basic::PolygonMesh::triMeshSubdivision()
{
    //subdivision非三角面片不能细分
    if (faces.cols() != 3)
    {
        cout << "subdivision can not subdivide non-triangular mesh!" << endl;
        return;
    }

    //生成无序边集
    set<Edge> unorderedEdges = creatUnorderedEdges();
    int rows = vertices.rows();
    //cout<<"edges.size:"<<edges.size()<<endl;
    //vertices.conservativeResize(rows+unorderedEdges.size(),3);//直接用resize会调用析构函数，里面的数据会消失
    //细分面片
    MatrixXd newVertices(rows + unorderedEdges.size(), 3); //细分后的顶点
    MatrixXi newFaces(faces.rows() * 4, 3);                 //细分后的面片
    //set<Point3d> points;
    map<Point3d, unsigned int> newVerticesIndex;
    int vertexIndex = 0;
    for (int i = 0; i < faces.rows(); i++)
    {
        vector<Point3d> curPoints; //记录当前面片顶点
        for (int j = 0; j < 3; j++)
        {
            //存放原有的顶点
            Point3d point(vertices.row(faces(i, j) - 1));
            curPoints.push_back(point);
            if (newVerticesIndex.count(point) == 0)
            {
                //cout<<"point:"<<point<<endl;
                //如果没有保存当前顶点(防止重复存储)
                newVertices.row(vertexIndex++) = vertices.row(faces(i, j) - 1); //这个下标从0开始
                newVerticesIndex[point] = vertexIndex;                         //注意这个下标从1开始
            }
        }

        //新增顶点
        Point3d newPoint1((vertices.row(faces(i, 0) - 1) + vertices.row(faces(i, 1) - 1)) / 2.0);
        if (newVerticesIndex.count(newPoint1) == 0)
        {
            newVertices.row(vertexIndex++) = newPoint1.data; //这个下标从0开始
            newVerticesIndex[newPoint1] = vertexIndex;      //注意这个下标从1开始
        }
        curPoints.push_back(newPoint1);

        Point3d newPoint2((vertices.row(faces(i, 1) - 1) + vertices.row(faces(i, 2) - 1)) / 2.0);
        if (newVerticesIndex.count(newPoint2) == 0)
        {
            newVertices.row(vertexIndex++) = newPoint2.data; //这个下标从0开始
            newVerticesIndex[newPoint2] = vertexIndex;      //注意这个下标从1开始
        }
        curPoints.push_back(newPoint2);

        Point3d newPoint3((vertices.row(faces(i, 2) - 1) + vertices.row(faces(i, 0) - 1)) / 2.0);
        if (newVerticesIndex.count(newPoint3) == 0)
        {
            newVertices.row(vertexIndex++) = newPoint3.data; //这个下标从0开始
            newVerticesIndex[newPoint3] = vertexIndex;      //注意这个下标从1开始
        }
        curPoints.push_back(newPoint3);

        //生成细分的面片
        //第一个细分面片
        newFaces(4 * i, 0) = newVerticesIndex[curPoints[0]];
        newFaces(4 * i, 1) = newVerticesIndex[curPoints[3]];
        newFaces(4 * i, 2) = newVerticesIndex[curPoints[5]];
        //第二个细分面片
        newFaces(4 * i + 1, 0) = newVerticesIndex[curPoints[1]];
        newFaces(4 * i + 1, 1) = newVerticesIndex[curPoints[3]];
        newFaces(4 * i + 1, 2) = newVerticesIndex[curPoints[4]];
        //第三个细分面片
        newFaces(4 * i + 2, 0) = newVerticesIndex[curPoints[2]];
        newFaces(4 * i + 2, 1) = newVerticesIndex[curPoints[4]];
        newFaces(4 * i + 2, 2) = newVerticesIndex[curPoints[5]];
        //第四个细分面片
        newFaces(4 * i + 3, 0) = newVerticesIndex[curPoints[3]];
        newFaces(4 * i + 3, 1) = newVerticesIndex[curPoints[4]];
        newFaces(4 * i + 3, 2) = newVerticesIndex[curPoints[5]];
    }

    // cout<<"newVertices: \n"<<newVertices<<endl;
    // cout<<"newVerticesIndex:\n";
    // for(auto iter=newVerticesIndex.begin();iter!=newVerticesIndex.end();iter++){
    //     cout<<iter->second<<endl;
    // }
    //块操作
    //newVertices.block(0,0,rows,3)=vertices;
    // cout<<"vertices:"<<vertices<<endl;
    // cout<<"newVertices:"<<newVertices<<endl;

    //生成odd vertices 注意下标
    // for(auto iter=unorderedEdges.begin();iter!=unorderedEdges.end();iter++){
    //     vertices.row(rows++)=(vertices.row(iter->firVerIndex-1)+vertices.row(iter->secVerIndex-1))/2.0;
    // }

    //顶点构建邻居数据结构
    // map<int,set<int> > neighbors;
    // for(int i=0;i<faces.rows();i++){
    //     for(int j=0;j<3;j++){
    //         for(int k=0;k<3;k++){
    //             if(j!=k){
    //                 neighbors[faces(i,j)].insert(faces(i,k));
    //             }
    //         }
    //     }
    // }

    //将新的vertices和faces赋值给mesh
    vertices = newVertices;
    faces = newFaces;

    // for(auto iter_map=neighbors.begin();iter_map!=neighbors.end();iter_map++){
    //     for(auto iter_set=iter_map->second.begin();iter_set!=iter_map->second.end();iter_set++){
    //         cout<<*iter_set;
    //     }
    //     cout<<endl;
    // }

    // cout<<"vertices:"<<endl;
    // cout<<vertices<<endl;
}

//loop细分
void wh::basic::PolygonMesh::loopSubdivision()
{
    //subdivision非三角面片不能细分
    if (faces.cols() != 3)
    {
        cout << "loop subdivision can not subdivide non-triangular mesh!" << endl;
        return;
    }
    //找到边集（无序边默认第一个索引比第二个小）
    set<Edge> unorderedEdges = creatUnorderedEdges();
}

//找到边的相邻面片
std::map<wh::basic::Edge, std::set<wh::basic::Face> >
wh::basic::PolygonMesh::findEdgeNearFaces(std::set<wh::basic::Edge> &edges)
{
    map<Edge, set<Face>> res; //结果
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        for (int i = 0; i < faces.rows(); i++)
        {
            if (is_exist(iter->firVerIndex, faces.row(i)) && is_exist(iter->secVerIndex, faces.row(i)))
            {
                res[*iter].insert(Face(faces.row(i)));
            }
        }
    }
    return res;
}

//判断vec中是否存在i
bool is_exist(const unsigned int i, const RowVector3i &vec)
{
    if (i == vec[0] || i == vec[1] || i == vec[2])
    {
        return true;
    }
    return false;
}