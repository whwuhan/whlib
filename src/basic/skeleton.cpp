#include <basic/skeleton.h>
#include <iostream>
#include <set>
#include <utils/io/io_obj.h>
using namespace std;
using namespace wh::basic;
using namespace wh::basic::adt;
using namespace wh::utils::io;
using namespace Eigen;
bool is_all_visited(vector<int> &flag);                                     //是否所有点都被访问
int get_next_degree_1_index(vector<int> &flag, vector<int> &points_degree); //获取下一个度为一的，没有被访问的点位置
wh::basic::Skeleton::Skeleton() : points(), edges()
{
}

wh::basic::Skeleton::Skeleton(Eigen::MatrixXd &points, Eigen::MatrixXi &edges) : points(points), edges(edges)
{
}

Eigen::MatrixXi wh::basic::Skeleton::get_adj_mat()
{
    int size = points.rows();
    MatrixXi adj_mat = MatrixXi::Zero(size, size);
    for (int row = 0; row < edges.rows(); row++)
    {
        adj_mat(edges(row, 0) - 1, edges(row, 1) - 1) = 1;
        adj_mat(edges(row, 1) - 1, edges(row, 0) - 1) = 1;
    }
    return adj_mat;
}

//模板方法实现
//显示特化要放到实现文件中，不能放到头文件中
//获取分支数组
template <>
std::vector<wh::basic::adt::Curve<wh::basic::Point3d>> wh::basic::Skeleton::get_branches()
{
    MatrixXi adj_mat = get_adj_mat(); //获取邻接矩阵

    //获取顶点的度
    vector<int> points_degree(points.rows(), 0);
    for (int row = 0; row < adj_mat.rows(); row++)
    {
        for (int col = 0; col < adj_mat.cols(); col++)
        {
            if (adj_mat(row, col) == 1)
            {
                points_degree[row]++;
            }
        }
    }
    // for(int i=0;i<points_degree.size();i++){
    //     cout << "points_degree" << i << ": " << points_degree[i] <<endl;
    // }

    //获取分支数组
    vector<Curve<Point3d>> res;
    vector<int> flag(points.rows(), 0); //当前顶点是否被访问

    int row = 0;
    while (get_next_degree_1_index(flag, points_degree) != -1)
    {
        int next_one_degree_index = get_next_degree_1_index(flag, points_degree);
        Curve<Point3d> curve;
        //获取下一个度为一的，没有被访问的点位置
        if (-1 != next_one_degree_index)
        {
            Point3d new_point(points.row(next_one_degree_index));
            curve.add_point_back(new_point);
            row = next_one_degree_index;
            flag[next_one_degree_index] = 1;
        }
        //获取一条分支
        while (true)
        {
            int last_row = row;
            for (int col = 0; col < adj_mat.cols(); col++)
            {
                if (adj_mat(row, col) == 1 && flag[col] == 0)
                {
                    Point3d new_point(points.row(col));
                    curve.add_point_back(new_point);
                    flag[col] = 1;
                    row = col;
                    break;
                }
            }
            // cout << "last_row " << last_row <<endl;
            // cout << "row " << row <<endl;
            if (last_row == row)
            {
                //curve.show();
                res.push_back(curve);
                // save_curves_obj("curve.obj",&res);
                break;
            }
        }
    }
    // save_curves_obj("curve.obj",&res);
    return res;
}

//获取相邻骨架点的平均间隔距离
double wh::basic::Skeleton::get_ave_interval()
{
    int edge_size = edges.rows();
    double dis_sum = get_tol_len();
    return dis_sum / edge_size;
}

double wh::basic::Skeleton::get_tol_len()
{
    int edge_size = edges.rows();
    double dis_sum = 0.0;
    for (int i = 0; i < edge_size; i++)
    {
        dis_sum += (points.row(edges(i, 0) - 1) - points.row(edges(i, 1) - 1)).norm();
        //cout << "dis_sum:" << dis_sum << endl;
    }
    return dis_sum;
}

//是否所有点都被访问
bool is_all_visited(vector<int> &flag)
{
    for (int i : flag)
    {
        if (i == 0)
        {
            return false;
        }
    }
    return true;
}

//获取下一个度为一的，没有被访问的点位置
int get_next_degree_1_index(vector<int> &flag, vector<int> &points_degree)
{
    for (int i = 0; i < flag.size(); i++)
    {
        if (flag[i] == 0 && points_degree[i] == 1)
        {
            return i;
        }
    }
    return -1;
}
