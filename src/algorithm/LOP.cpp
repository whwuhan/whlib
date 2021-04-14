#include <algorithm/LOP.h>
#include <basic/point_cloud.h>
#include <basic/point3d.h>
#include <utils/string_lib.h>
#include <cmath>
using namespace std;
using namespace Eigen;
using namespace wh::basic;
using namespace wh::alg;
using namespace wh::utils::io;
using namespace wh::utils;
//构造函数
LOP_parameters::LOP_parameters() : iter(10), h(1.0), mu(0.45), amount(1000){}

LOP_parameters::LOP_parameters(int iter, int amount, double r, double h, double mu) : 
iter(iter), amount(amount), h(h), mu(mu){}

LOP::LOP() {}

LOP::LOP(string &file_name, MatrixXd &X, MatrixXd &P, LOP_PARAMETERS &parameters) : 
file_name(file_name), X(X), P(P), parameters(parameters){
    PointCloud point_cloud;
    load_point_cloud_obj(file_name, &point_cloud);
    P = point_cloud.points;
}

void LOP::set_file_name(const string file_name){
    this->file_name = file_name;
}

void LOP::init(){
    if (file_name == ""){
        cout << "init() 请先设置原始点云路径" << endl;
        return;
    }

    auto file_name_split = split(file_name, "./\\");
    string out_file_name = file_name_split[file_name_split.size() - 2];
    PointCloud point_cloud;
    load_point_cloud_obj(file_name, &point_cloud);
    P = point_cloud.points;

    if (P.rows() != 0 && P.cols() != 0){
        X = MatrixXd::Random(parameters.amount, P.cols()) * P.maxCoeff();
        string rand_save_name = out_file_name + "_rand.obj";
        cout << "随机初始化点云保存到文件:" << rand_save_name << endl;
        PointCloud rand_point_cloud(X);
        save_point_cloud_obj(rand_save_name, &rand_point_cloud);

        MatrixXd X = get_x_prime1(); //从随机点得到初始迭代点
        cout << "正在获取初始迭代点" << endl;
        string init_save_name = out_file_name + "_init.obj";
        cout << "初始迭代点云保存到文件:" << init_save_name << endl;
        PointCloud init_point_cloud(X);
        save_point_cloud_obj(init_save_name, &init_point_cloud);
    }else{
        cerr << "init() LOP初始化失败，没有获取原始点云" << endl;
    }
}

void LOP::run(){
    if (P.rows() == 0 || P.cols() == 0){
        cerr << "run() 没有原始点云" << endl;
        return;
    }

    if (X.rows() == 0 || X.cols() == 0){
        cerr << "run() 没有使用init()初始化参数" << endl;
        return;
    }

    auto file_name_split = split(file_name, "./\\");
    string out_file_name = file_name_split[file_name_split.size() - 2];
    for (int iter = 0; iter < parameters.iter; iter++){
        cout << "第" << iter + 1 << "次迭代开始" << endl;
        X = LOP_outer(X, P);
        string save_name = out_file_name + "_iter_" + to_string(iter + 1) + ".obj";
        cout << "第" << iter + 1 << "次迭代结束，保存到文件" << save_name << endl;
        PointCloud point_cloud(X);
        save_point_cloud_obj(save_name, &point_cloud);
    }
}

double LOP::theta(double r){
    return exp((-pow(r, 2)) / pow(parameters.h / 4, 2));
}

double LOP::eta(double r){
    return 1 / 3 * pow(r, 3);
}

MatrixXd LOP::get_x_prime1(){
    int x_size = X.rows();
    int p_size = P.rows();
    RowVector3d res_top;
    double res_bottom = 0.0;
    MatrixXd X_Prime(X.rows(), X.cols());
    for (int i = 0; i < x_size; i++){
        for (int j = 0; j < p_size; j++){
            res_top += P.row(j) * theta((P.row(j) - X.row(i)).norm());
            res_bottom += theta((P.row(j) - X.row(i)).norm());
        }
        X_Prime.row(i) = res_top / res_bottom;
        res_top = RowVector3d(0, 0, 0);
        res_bottom = 0.0;
    }
    // cout<<"get_x_prime1"<<endl;
    return X_Prime;
}

double LOP::get_alpha(const RowVector3d &x_prime, const RowVector3d &p){
    double res_top = theta((x_prime - p).norm());
    double res_bottom = (x_prime - p).norm();
    // cout<<"get_alpha"<<endl;
    return res_top / res_bottom;
}

double LOP::get_beta(const RowVector3d &x, const RowVector3d &x_prime){
    double res_top = theta((x_prime - x).norm());
    double res_bottom = (x_prime - x).norm();
    // cout<<"get_beta"<<endl;
    return res_top * res_bottom;
}

RowVector3d LOP::LOP_inner(const RowVector3d &x_prime){
    RowVector3d fir;
    RowVector3d sec;
    int x_size = X.rows();
    int p_size = P.rows();
    // cout<<"get_alpha"<<endl;
    double alphaTotal = 0.0;
    for (int j = 0; j < p_size; j++){
        alphaTotal += get_alpha(x_prime, P.row(j));
    }
    for (int j = 0; j < p_size; j++){
        fir += (P.row(j) * get_alpha(x_prime, P.row(j))) / alphaTotal;
    }

    double beta_total = 0.0;
    for (int i = 0; i < x_size; i++){
        if (x_prime != X.row(i)){
            beta_total += get_beta(X.row(i), x_prime);
        }
    }
    for (int i = 0; i < x_size; i++){
        if (x_prime != X.row(i)){
            sec += (x_prime - X.row(i)) * get_beta(X.row(i), x_prime) / beta_total;
        }
    }
    // cout<<"LOP_inner"<<endl;
    return fir + parameters.mu * sec;
}

MatrixXd LOP::LOP_outer(const MatrixXd &X, const MatrixXd &P){
    int x_size = X.rows();
    MatrixXd X_prime_K_add_one(X.rows(), X.cols());
    for (int i = 0; i < x_size; i++){
        X_prime_K_add_one.row(i) = LOP_inner(X.row(i));
    }
    return X_prime_K_add_one;
}