/**
 * 曲线抽象结构
 * 2020.08.21
 * wuhan
*/
#pragma once
#include <vector>
namespace wh
{
    namespace basic
    {
        template <typename T>
        class Curve
        {
        public:
            Curve();
            Curve(const std::vector<T> &points);
            std::vector<T> points; //一条曲线上按顺序排列的点
            unsigned int size;     //曲线点的个数

            std::vector<T> &get_points(); //获取曲线所有点数据
            unsigned int get_size(); //获取曲线点的个数
            void resize(const unsigned int new_size); //设置点数量
            void show(); //显示曲线点的信息
            double get_len(); //获取曲线长度
            std::vector<T> get_end_points(); //获取曲线的两端点
            void add_point_back(T &t); //在曲线的最后增加点
            void add_point_front(T &t); //在曲线的最前面加点
            void add_points_back(std::vector<T> &points); //在曲线后面插入多个点
            void add_points_front(std::vector<T> &points); //在曲线前面插入多个点
        };

        //构造函数
        template <typename T>
        Curve<T>::Curve() : points(), size(0) {}

        template <typename T>
        Curve<T>::Curve(const std::vector<T> &points) : points(points), size(points.size()){}

        //get函数
        template <typename T>
        std::vector<T> &Curve<T>::get_points()
        {
            return points;
        }

        template <typename T>
        unsigned int Curve<T>::get_size()
        {
            return size;
        }

        //设置点数量
        template <typename T>
        void Curve<T>::resize(const unsigned int new_size)
        {
            size = new_size;
            points.resize(new_size);
        }

        //显示曲线点的信息
        template <typename T>
        void Curve<T>::show()
        {
            if (size <= 0)
            {
                std::cout << "no points data." << std::endl;
                return;
            }
            std::cout << "Curve's size: " << size << std::endl;
            for (auto it = points.cbegin(); it != points.cend(); it++)
            {
                std::cout << *it << std::endl;
            }
        }

        //获取曲线长度
        template <typename T>
        double Curve<T>::get_len()
        {
            double len; //长度
            for (int i = 0; i < size - 1; i++)
            {
                //std::cout<<(points[i].data - points[i+1].data).norm()<<std::endl;
                len += (points[i].data - points[i + 1].data).norm();
            }
            //std::cout<<len<<std::endl;
            return len;
        }

        //在曲线的最后增加点
        template <typename T>
        void Curve<T>::add_point_back(T &t)
        {
            T newPoint(t);
            points.push_back(newPoint);
            this->size++;
        }

        //在曲线的最前面加点
        template <typename T>
        void Curve<T>::add_point_front(T &t)
        {
            T newPoint(t);
            points.emplace(points.begin(), newPoint);
            this->size++;
        }

        //在曲线后面插入多个点
        template <typename T>
        void Curve<T>::add_points_back(std::vector<T> &points)
        {
            int addPointsSize = points.size();
            for (T t : points)
            {
                this->points.push_back(t);
            }
            this->size += addPointsSize;
        }

        //在曲线前面插入多个点
        template <typename T>
        void Curve<T>::add_points_front(std::vector<T> &points)
        {
            int addPointsSize = points.size();
            for (int i = addPointsSize - 1; i > -1; i--)
            {
                this->points.emplace(this->points.begin(), points[i]);
            }
            this->size += addPointsSize;
        }

        //获取曲线的两端点
        template <typename T>
        std::vector<T> Curve<T>::get_end_points()
        {
            std::vector<T> res(2);
            res[0] = points[0];
            res[1] = points[points.size() - 1];
            return res;
        }
    }// end namespace basic
} // end namespace wh