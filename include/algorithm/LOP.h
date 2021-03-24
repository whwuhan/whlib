#ifndef LOP_H
#define LOP_H
#include <Eigen/Dense>
#include <utils/io/io_obj.h>
namespace wh{
    namespace alg{
        //LOP参数
        typedef struct LOP_parameters{
            int iter;   //迭代次数
            int amount; //采样点数
            double h;   //theta范围半径
            double mu;  //不动点迭代权重
            LOP_parameters();
            LOP_parameters(int iter, int amount, double r, double h, double mu);
        } LOP_PARAMETERS;
        
        class LOP{
        public:
            LOP();
            LOP(std::string &file_name,
                Eigen::MatrixXd &X,
                Eigen::MatrixXd &P,
                LOP_parameters &parameters);
            
            std::string file_name;           //点云文件路径
            Eigen::MatrixXd X;              //初始点云
            Eigen::MatrixXd P;              //采样原始点云
            struct LOP_parameters parameters; //其他参数
            void set_file_name(const std::string file_name);
            void init(); //初始化
            void run();  //开始运行
        private:
            double theta(double r);
            double eta(double r);
            Eigen::MatrixXd get_x_prime1();
            double get_alpha(const Eigen::RowVector3d &x_prime, const Eigen::RowVector3d &p);
            double get_beta(const Eigen::RowVector3d &x, const Eigen::RowVector3d &x_prime);
            Eigen::RowVector3d LOP_inner(const Eigen::RowVector3d &x_prime);
            Eigen::MatrixXd LOP_outer(const Eigen::MatrixXd &X, const Eigen::MatrixXd &P);
        };
    } // namespace alg
} // namespace wh

#endif