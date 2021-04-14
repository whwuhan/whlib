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
using namespace glm;
bool is_exist(const unsigned int i, const RowVector3i &vec); //判断vec中是否存在i
//构造函数
PolygonMesh::PolygonMesh() : 
vertices(), vertices_indices(), VAO(0), VBO(0), EBO(0), show(true), 
face_color(0.6f, 0.6f, 0.6f, 1.0f), point_line_color(0.0f, 0.0f, 0.0f, 1.0f), model(1.0f),
scale(1.0f),
trans_x(0.0f),
trans_y(0.0f),
trans_z(0.0f),
rotate_x(0.0f),
rotate_y(0.0f),
rotate_z(0.0f),
point_size(5.0f){}
    
PolygonMesh::PolygonMesh(MatrixXd &vertices, MatrixXi &vertices_indices) : 
vertices(vertices), vertices_indices(vertices_indices), VAO(0), VBO(0), EBO(0), show(true), 
face_color(0.6f, 0.6f, 0.6f, 1.0f), point_line_color(0.0f, 0.0f, 0.0f, 1.0f), model(1.0f),
scale(1.0f),
trans_x(0.0f),
trans_y(0.0f),
trans_z(0.0f),
rotate_x(0.0f),
rotate_y(0.0f),
rotate_z(0.0f),
point_size(5.0f){}

set<Edge> PolygonMesh::creat_unordered_edges(){
    set<Edge> res;
    for (int i = 0; i < vertices_indices.rows(); i++){
        res.insert(Edge(vertices_indices(i, 0), vertices_indices(i, 1)));
        res.insert(Edge(vertices_indices(i, 1), vertices_indices(i, 2)));
        res.insert(Edge(vertices_indices(i, 2), vertices_indices(i, 0)));
    }
    return res;
}

std::set<Edge> PolygonMesh::creat_ordered_edges(){
    set<Edge> res;
    for (int i = 0; i < vertices_indices.rows(); i++){
        res.insert(Edge(vertices_indices(i, 0), vertices_indices(i, 1), false));
        res.insert(Edge(vertices_indices(i, 1), vertices_indices(i, 2), false));
        res.insert(Edge(vertices_indices(i, 2), vertices_indices(i, 0), false));
    }
    return res;
}

//普通三角细分
void PolygonMesh::tri_mesh_subdivision(){
    //subdivision非三角面片不能细分
    if (vertices_indices.cols() != 3){
        cout << "subdivision can not subdivide non-triangular mesh!" << endl;
        return;
    }

    //生成无序边集
    set<Edge> unordered_edges = creat_unordered_edges();
    int rows = vertices.rows();
    //cout<<"edges.size:"<<edges.size()<<endl;
    //vertices.conservative_resize(rows+unordered_edges.size(),3);//直接用resize会调用析构函数，里面的数据会消失
    //细分面片
    MatrixXd new_vertices(rows + unordered_edges.size(), 3); //细分后的顶点
    MatrixXi new_vertices_indices(vertices_indices.rows() * 4, 3);                 //细分后的面片
    //set<Point3d> points;
    map<Point3d, unsigned int> new_vertices_index;
    int vertex_index = 0;
    for (int i = 0; i < vertices_indices.rows(); i++){
        vector<Point3d> cur_points; //记录当前面片顶点
        for (int j = 0; j < 3; j++){
            //存放原有的顶点
            Point3d point(vertices.row(vertices_indices(i, j) - 1));
            cur_points.push_back(point);
            if (new_vertices_index.count(point) == 0){
                //cout<<"point:"<<point<<endl;
                //如果没有保存当前顶点(防止重复存储)
                new_vertices.row(vertex_index++) = vertices.row(vertices_indices(i, j) - 1); //这个下标从0开始
                new_vertices_index[point] = vertex_index;                         //注意这个下标从1开始
            }
        }

        //新增顶点
        Point3d new_point1((vertices.row(vertices_indices(i, 0) - 1) + vertices.row(vertices_indices(i, 1) - 1)) / 2.0);
        if (new_vertices_index.count(new_point1) == 0){
            new_vertices.row(vertex_index++) = new_point1.data; //这个下标从0开始
            new_vertices_index[new_point1] = vertex_index;      //注意这个下标从1开始
        }
        cur_points.push_back(new_point1);

        Point3d new_point2((vertices.row(vertices_indices(i, 1) - 1) + vertices.row(vertices_indices(i, 2) - 1)) / 2.0);
        if (new_vertices_index.count(new_point2) == 0){
            new_vertices.row(vertex_index++) = new_point2.data; //这个下标从0开始
            new_vertices_index[new_point2] = vertex_index;      //注意这个下标从1开始
        }
        cur_points.push_back(new_point2);

        Point3d new_point3((vertices.row(vertices_indices(i, 2) - 1) + vertices.row(vertices_indices(i, 0) - 1)) / 2.0);
        if (new_vertices_index.count(new_point3) == 0){
            new_vertices.row(vertex_index++) = new_point3.data; //这个下标从0开始
            new_vertices_index[new_point3] = vertex_index;      //注意这个下标从1开始
        }
        cur_points.push_back(new_point3);

        //生成细分的面片
        //第一个细分面片
        new_vertices_indices(4 * i, 0) = new_vertices_index[cur_points[0]];
        new_vertices_indices(4 * i, 1) = new_vertices_index[cur_points[3]];
        new_vertices_indices(4 * i, 2) = new_vertices_index[cur_points[5]];
        //第二个细分面片
        new_vertices_indices(4 * i + 1, 0) = new_vertices_index[cur_points[1]];
        new_vertices_indices(4 * i + 1, 1) = new_vertices_index[cur_points[3]];
        new_vertices_indices(4 * i + 1, 2) = new_vertices_index[cur_points[4]];
        //第三个细分面片
        new_vertices_indices(4 * i + 2, 0) = new_vertices_index[cur_points[2]];
        new_vertices_indices(4 * i + 2, 1) = new_vertices_index[cur_points[4]];
        new_vertices_indices(4 * i + 2, 2) = new_vertices_index[cur_points[5]];
        //第四个细分面片
        new_vertices_indices(4 * i + 3, 0) = new_vertices_index[cur_points[3]];
        new_vertices_indices(4 * i + 3, 1) = new_vertices_index[cur_points[4]];
        new_vertices_indices(4 * i + 3, 2) = new_vertices_index[cur_points[5]];
    }

    // cout<<"new_vertices: \n"<<new_vertices<<endl;
    // cout<<"new_vertices_index:\n";
    // for(auto iter=new_vertices_index.begin();iter!=new_vertices_index.end();iter++){
    //     cout<<iter->second<<endl;
    // }
    //块操作
    //new_vertices.block(0,0,rows,3)=vertices;
    // cout<<"vertices:"<<vertices<<endl;
    // cout<<"new_vertices:"<<new_vertices<<endl;

    //生成odd vertices 注意下标
    // for(auto iter=unordered_edges.begin();iter!=unordered_edges.end();iter++){
    //     vertices.row(rows++)=(vertices.row(iter->fir_ver_index-1)+vertices.row(iter->sec_ver_index-1))/2.0;
    // }

    //顶点构建邻居数据结构
    // map<int,set<int> > neighbors;
    // for(int i=0;i<vertices_indices.rows();i++){
    //     for(int j=0;j<3;j++){
    //         for(int k=0;k<3;k++){
    //             if(j!=k){
    //                 neighbors[vertices_indices(i,j)].insert(vertices_indices(i,k));
    //             }
    //         }
    //     }
    // }

    //将新的vertices和vertices_indices赋值给mesh
    vertices = new_vertices;
    vertices_indices = new_vertices_indices;

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
// TODO
void PolygonMesh::loop_subdivision(){
    //subdivision非三角面片不能细分
    if (vertices_indices.cols() != 3){
        cout << "loop subdivision can not subdivide non-triangular mesh!" << endl;
        return;
    }
    //找到边集（无序边默认第一个索引比第二个小）
    set<Edge> unordered_edges = creat_unordered_edges();
}

//找到边的相邻面片
map<Edge, set<Face> >
PolygonMesh::find_edge_near_faces(std::set<Edge> &edges){
    map<Edge, set<Face>> res; //结果
    for (auto iter = edges.begin(); iter != edges.end(); iter++){
        for (int i = 0; i < vertices_indices.rows(); i++){
            if (is_exist(iter->fir_ver_index, vertices_indices.row(i)) && is_exist(iter->sec_ver_index, vertices_indices.row(i))){
                res[*iter].insert(Face(vertices_indices.row(i)));
            }
        }
    }
    return res;
}

//判断vec中是否存在i
bool is_exist(const unsigned int i, const RowVector3i &vec){
    if (i == vec[0] || i == vec[1] || i == vec[2]){
        return true;
    }
    return false;
}