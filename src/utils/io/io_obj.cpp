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
void wh::utils::io::load_point_cloud_obj(const string file_name, struct PointCloud *point_cloud_ptr){
    //打开文件
    ifstream data_src(file_name);
    if (!data_src.is_open()){
        cout << "no data source." << endl;
        return;
    }
    
    //读入文件
    string line;
    vector<string> line_split;
    unsigned int count = 0; //obj文件点的数量

    while (getline(data_src, line)){
        line_split = split(line, " ");
        if(line_split.size() != 0){
            if(line_split[0] == "v"){
                count++;
            }
        }
    }
    cout << "Point Cloud size:" << count << endl;
    data_src.clear(); //先要clear()才能回到文件头
    data_src.seekg(0, ios::beg);
    point_cloud_ptr->resize(count, 3);
    count = 0;

    while (getline(data_src, line)){
        line_split = split(line, " ");
        if(line_split.size() != 0){//防止空行
            if(line_split[0] == "v"){
                point_cloud_ptr->points(count, 0) = stof(line_split[1]);
                point_cloud_ptr->points(count, 1) = stof(line_split[2]);
                point_cloud_ptr->points(count, 2) = stof(line_split[3]);
                count++;
            }
        }
    }
    cout << "Load Point Cloud Successfully!" << endl;
    data_src.close();
}
        
//点云存入obj文件
void wh::utils::io::save_point_cloud_obj(const string file_name, const struct PointCloud *const point_cloud_ptr){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib point cloud obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入数据
    for (int i = 0; i < point_cloud_ptr->size; i++){
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << point_cloud_ptr->points.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << point_cloud_ptr->points.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << point_cloud_ptr->points.row(i)[2] << endl;
    }
    cout << "Save Point Cloud Successfully!" << endl;
    data_des.close();
}

//cube triangular mesh 写入obj文件
void wh::utils::io::save_tri_cube_mesh_obj(const string file_name, Cube *cube_ptr){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cube_ptr->vertices.rows(); i++){
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[2] << endl;
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
void wh::utils::io::save_tri_cube_meshes_obj(const string file_name, const vector<Cube> &cubes){
    //打开文件
    ofstream data_des(file_name);

    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes){
        //cout<<cube.vertices.size()<<endl;
        for (int i = 0; i < cube.vertices.rows(); i++){
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //存入面数据
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

//重载保存triangular cube meshes
void wh::utils::io::save_tri_cube_meshes_obj(const string file_name, const set<Cube> &cubes){
    //打开文件
    ofstream data_des(file_name);

    data_des << "# whlib cube mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

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
void wh::utils::io::save_cube_wireframe_obj(const string file_name, Cube *cube_ptr){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib cube wireframe obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入点数据
    for (int i = 0; i < cube_ptr->vertices.rows(); i++){
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube_ptr->vertices.row(i)[2] << endl;
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
void wh::utils::io::save_cube_wireframes_obj(const string file_name, const vector<Cube> &cubes){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes){
        for (int i = 0; i < cube.vertices.rows(); i++){
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++){
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

void wh::utils::io::save_cube_wireframes_obj(const string file_name, const set<Cube> &cubes){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib cube wireframes obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    //存入点数据
    for (Cube cube : cubes){
        for (int i = 0; i < cube.vertices.rows(); i++){
            data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[0];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[1];
            data_des << " " << setiosflags(ios::fixed) << setprecision(10) << cube.vertices.row(i)[2] << endl;
        }
    }

    //写入线框（注意顺序首个索引从小到大）
    for (int i = 0; i < cubes.size(); i++){
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
void wh::utils::io::load_polygon_mesh_obj(const string file_name, PolygonMesh *polygon_mesh_ptr){
    //打开文件
    ifstream data_src(file_name);
    if (!data_src.is_open()){
        cout << "no data source." << endl;
        return;
    }

    //读入文件
    string line;
    vector<string> line_split;
    unsigned int vertices_amount = 0; //顶点数量
    unsigned int faces_amount = 0;  //面片数量
    unsigned int uv_amount = 0;//纹理坐标数量
    unsigned int normals_amount = 0;//法线数量

    int polygon_type = 3;                     //polygon mesh的类型（三角面片还是四边形面片，默认是三角面片）

    //获取数量
    while (getline(data_src, line)){
        line_split = split(line, " ");
        if(line_split.size() != 0){
            if(line_split[0] == "v") vertices_amount++;
            if(line_split[0] == "vt") uv_amount++;
            if(line_split[0] == "vn") normals_amount++;
            if(line_split[0] == "f") faces_amount++;
        }
    }

    data_src.clear(); //先要clear()才能回到文件头
    data_src.seekg(0, ios::beg);
    //注意 uv坐标 normal 的数量可能不等于面片的数量
    //分配空间
    polygon_mesh_ptr->vertices.resize(vertices_amount, 3);
    polygon_mesh_ptr->vertices_indices.resize(faces_amount, polygon_type);
    polygon_mesh_ptr->uvs.resize(uv_amount, 2);
    polygon_mesh_ptr->uv_indices.resize(faces_amount, polygon_type);
    polygon_mesh_ptr->normals.resize(normals_amount, 3);
    polygon_mesh_ptr->normals_indices.resize(faces_amount, polygon_type);
    

    cout << "vertices_amount:" << vertices_amount << endl;
    cout << "faces_amount:" << faces_amount << endl;
    cout << "uv_amount:" << uv_amount << endl;
    cout << "normals_amount:" << normals_amount << endl;

    vertices_amount = 0;
    faces_amount = 0;
    uv_amount = 0;
    normals_amount = 0;
    
    //读取数据
    while (getline(data_src, line)){
        line_split = split(line, " ");
        if(line_split.size() != 0){
            //顶点
            if(line_split[0] == "v"){
                polygon_mesh_ptr->vertices(vertices_amount, 0) = stof(line_split[1]);
                polygon_mesh_ptr->vertices(vertices_amount, 1) = stof(line_split[2]);
                polygon_mesh_ptr->vertices(vertices_amount, 2) = stof(line_split[3]);
                vertices_amount++;
            }
            //顶点UV坐标
            if(line_split[0] == "vt"){
                polygon_mesh_ptr->uvs(uv_amount, 0) = stof(line_split[1]);
                polygon_mesh_ptr->uvs(uv_amount, 1) = stof(line_split[2]);
                uv_amount++;
            }
            //顶点法线
            if(line_split[0] == "vn"){
                polygon_mesh_ptr->normals(normals_amount, 0) = stof(line_split[1]);
                polygon_mesh_ptr->normals(normals_amount, 1) = stof(line_split[2]);
                polygon_mesh_ptr->normals(normals_amount, 2) = stof(line_split[3]);
                normals_amount++;
            }
            vector<string> indices_split1 = split(line_split[1], "/");
            vector<string> indices_split2 = split(line_split[2], "/");
            vector<string> indices_split3 = split(line_split[3], "/");
            //面片包含 vertex/uv/normals
            if(line_split[0] == "f" && split(line_split[1], "/").size() == 3 )
            {   
                //面片顶点索引
                polygon_mesh_ptr->vertices_indices(faces_amount, 0) = stoi(indices_split1[0]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 1) = stoi(indices_split2[0]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 2) = stoi(indices_split3[0]);
                //面片UV坐标索引
                polygon_mesh_ptr->uv_indices(faces_amount, 0) = stoi(indices_split1[1]);
                polygon_mesh_ptr->uv_indices(faces_amount, 1) = stoi(indices_split2[1]);
                polygon_mesh_ptr->uv_indices(faces_amount, 2) = stoi(indices_split3[1]);
                //面片顶点法线索引
                polygon_mesh_ptr->normals_indices(faces_amount, 0) = stoi(indices_split1[2]);
                polygon_mesh_ptr->normals_indices(faces_amount, 1) = stoi(indices_split2[2]);
                polygon_mesh_ptr->normals_indices(faces_amount, 2) = stoi(indices_split3[2]);
                faces_amount++;
            }
            //面片信息只有顶点坐标和法线
            else if(line_split[0] == "f" && split(line_split[1], "/").size() == 2)
            {
                //面片顶点索引
                polygon_mesh_ptr->vertices_indices(faces_amount, 0) = stoi(indices_split1[0]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 1) = stoi(indices_split2[0]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 2) = stoi(indices_split3[0]);
                //面片顶点法线索引
                polygon_mesh_ptr->normals_indices(faces_amount, 0) = stoi(indices_split1[1]);
                polygon_mesh_ptr->normals_indices(faces_amount, 1) = stoi(indices_split2[1]);
                polygon_mesh_ptr->normals_indices(faces_amount, 2) = stoi(indices_split3[1]);
                faces_amount++;
            }
            else if(line_split[0] == "f")//面片只包含vertex index
            {
                polygon_mesh_ptr->vertices_indices(faces_amount, 0) = stoi(line_split[1]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 1) = stoi(line_split[2]);
                polygon_mesh_ptr->vertices_indices(faces_amount, 2) = stoi(line_split[3]);
            }
        }
    }
    // cout << polygon_mesh_ptr->vertices_indices.rows() << endl;
    // cout << polygon_mesh_ptr->uv_indices.rows() << endl;
    // cout << polygon_mesh_ptr->normals_indices.rows() << endl;
    // cout << polygon_mesh_ptr->vertices.rows() << endl;
    // cout << polygon_mesh_ptr->UVs.rows() << endl;
    // cout << polygon_mesh_ptr->normals.rows() << endl;
    cout << "Load Polygon Mesh Successfully!" << endl;
    data_src.close();
}

//保存Polygon mesh
void wh::utils::io::save_polygon_mesh_obj(const string file_name, PolygonMesh *polygon_mesh_ptr){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib polygon mesh obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象
    cout << "vertices_amount:" << polygon_mesh_ptr->vertices.rows() << endl;
    cout << "faces_amount:" << polygon_mesh_ptr->vertices_indices.rows() << endl;
    //存入点数据
    for (int i = 0; i < polygon_mesh_ptr->vertices.rows(); i++){
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << polygon_mesh_ptr->vertices.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << polygon_mesh_ptr->vertices.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << polygon_mesh_ptr->vertices.row(i)[2] << endl;
    }
    //写入面片信息
    int polygon_mesh_type = polygon_mesh_ptr->vertices_indices.cols(); //获取是三角面片是是正方形面片
    for (int i = 0; i < polygon_mesh_ptr->vertices_indices.rows(); i++){
        data_des << "f";
        for (int j = 0; j < polygon_mesh_type; j++){
            //cout<<"face:"<<polygon_mesh_ptr->verticesIndices.row(i)[j]<<endl;
            data_des << " " << polygon_mesh_ptr->vertices_indices.row(i)[j];
        }
        data_des << endl;
    }
    cout << "Save Polygon Mesh Successfully!" << endl;
    data_des.close();
}

// load skeleton obj
void wh::utils::io::load_skeleton_obj(const string file_name, Skeleton *skeleton_ptr){
    //打开文件
    ifstream data_src(file_name);
    if (!data_src.is_open()){
        cout << "no data source." << endl;
        return;
    }

    //读入文件
    string line;
    vector<string> line_split;
    unsigned int points_amount = 0; //骨架点数量
    unsigned int edges_amount = 0;  //连线数量

    //确定数量
    while (getline(data_src, line)){
        if (line[0] == 'v'){
            points_amount++;
        }
        if (line[0] == 'l'){
            edges_amount++;
        }
    }

    data_src.clear(); //先要clear()才能回到文件头
    data_src.seekg(0, ios::beg);

    cout << "points_amount:" << points_amount << endl;
    cout << "edges_amount:" << edges_amount << endl;

    skeleton_ptr->points.resize(points_amount, 3);
    skeleton_ptr->edges.resize(edges_amount, 2);

    points_amount = 0;
    edges_amount = 0;
    while (getline(data_src, line)){
        switch (line[0]){
            case 'v':
                line_split = split(line, " ");
                skeleton_ptr->points(points_amount, 0) = atof(line_split[1].c_str());
                skeleton_ptr->points(points_amount, 1) = atof(line_split[2].c_str());
                skeleton_ptr->points(points_amount, 2) = atof(line_split[3].c_str());
                points_amount++;
                break;
            case 'l':
                line_split = split(line, " ");
                skeleton_ptr->edges(edges_amount, 0) = atoi(line_split[1].c_str());
                skeleton_ptr->edges(edges_amount, 1) = atoi(line_split[2].c_str());
                edges_amount++;
                break;
            default:
                cout << line << endl;
        }
    }
    cout << "Load Skeleton Successfully!" << endl;
    data_src.close();
}

// save skeleton obj
void wh::utils::io::save_skeleton_obj(const string file_name, Skeleton *skeleton_ptr){
    //打开文件
    ofstream data_des(file_name);
    data_des << "# whlib Skeleton obj file" << endl; //文件头注释

    //获取当地时间
    time_t now = time(0);
    string date_time(ctime(&now));

    //注意时间后面自带换行
    data_des << "# " << date_time; //写入存储时间

    //存储对象名
    vector<string> file_name_split = wh::utils::split(file_name, "/.\\");
    int file_name_index = file_name_split.size() - 2;
    data_des << "o " << file_name_split[file_name_index] << endl; //obj对象

    int points_size = skeleton_ptr->points.rows();
    for (int i = 0; i < points_size; i++){ //写入骨架点
        data_des << "v" << " " << setiosflags(ios::fixed) << setprecision(10) << skeleton_ptr->points.row(i)[0];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << skeleton_ptr->points.row(i)[1];
        data_des << " " << setiosflags(ios::fixed) << setprecision(10) << skeleton_ptr->points.row(i)[2] << endl;
    }

    int edges_size = skeleton_ptr->edges.rows();
    for (int i = 0; i < edges_size; i++){
        data_des << "l" << " " << skeleton_ptr->edges(i, 0);
        data_des << " " << skeleton_ptr->edges(i, 1) << endl;
    }
    cout << "Save Skeleton Successfully!" << endl;
    data_des.close();
}