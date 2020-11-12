#ifndef IO_H
#define IO_H
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <set>
#include <basic/point_cloud.h>
#include <basic/skeleton.h>
#include <basic/abstract_data_type/graph.h>
#include <basic/abstract_data_type/curve.h>
#include <basic/polygon_mesh.h>
#include <utils/string_lib.h>
#include <utils/io/io_obj.h>
namespace wh{
    namespace utils{   
        namespace io{
            //======声明=========
            //点云Point_cloud
            void load_point_cloud_obj(const std::string file_name,struct wh::basic::Point_cloud* point_cloud_ptr);
            void save_point_cloud_obj(const std::string file_name,const struct wh::basic::Point_cloud* const point_cloud_ptr);
            
            //图Graph
            template <typename T>
            void load_graph_obj(const std::string file_name,wh::basic::adt::Graph<T>* graph_ptr);
            template <typename T>
            void save_graph_obj(const std::string file_name,const wh::basic::adt::Graph<T>* const graph_ptr);
            
            //曲线curve
            //保存一系列曲线
            template <typename T>
            void save_curves_obj(const std::string file_name,const std::vector< wh::basic::adt::Curve<T> >* const curves_ptr);
            //保存一条曲线
            template <typename T>
            void save_curve_obj(const std::string file_name,std::vector< wh::basic::adt::Curve<T> >* curves_ptr);
            //读取一条曲线
            template <typename T>
            void load_curve_obj(const std::string file_name,wh::basic::adt::Curve<T>* curve_ptr);


            //存储立方体cube mesh
            //存储为三角网格模型
            void save_tri_cube_mesh_obj(const std::string file_name,wh::basic::Cube* cube_ptr);
            void save_tri_cube_meshes_obj(const std::string file_name,const std::vector<wh::basic::Cube>& cubes);
            void save_tri_cube_meshes_obj(const std::string file_name,const std::set<wh::basic::Cube>& cubes);
            void save_cube_wireframe_obj(const std::string file_name,wh::basic::Cube* cube_ptr);
            void save_cube_wireframes_obj(const std::string file_name,const std::vector<wh::basic::Cube>& cubes);
            void save_cube_wireframes_obj(const std::string file_name,const std::set<wh::basic::Cube>& cubes);

            //polygon mesh
            void load_polygon_mesh_obj(const std::string file_name,wh::basic::Polygon_mesh *polygon_mesh_ptr);
            void save_polygon_mesh_obj(const std::string file_name,wh::basic::Polygon_mesh *polygon_mesh_ptr);
            
            //skeleton 骨架线
            void load_skeleton_obj(const std::string file_name,wh::basic::Skeleton *skeleton_ptr);
            void save_skeleton_obj(const std::string file_name,wh::basic::Skeleton *skeleton_ptr);

            //=========模板方法实现=========
            template <typename T>
            void save_curves_obj(const std::string file_name, std::vector< wh::basic::adt::Curve<T> >* const curves_ptr){
                //打开文件
                std::ofstream data_destination(file_name);
                data_destination << "# whlib skeleton obj file" << std::endl;//文件头注释
                
                //获取当地时间
                time_t now = time(0);
                std::string date_time(ctime(&now));

                //注意时间后面自带换行
                data_destination << "# " << date_time;//写入存储时间
                data_destination << "o " << date_time;//以时间命名obj对象

                //存入数据
                //先存点
                for(auto it = curves_ptr->begin(); it != curves_ptr->end(); it++){
                    const std::vector<T>& points = it->get_points();
                    for(unsigned int i = 0; i < it->get_size(); i++){
                        data_destination << "v" << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[0];
                        data_destination << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[1];
                        data_destination << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[2] << std::endl;
                    }
                }
                //再存边
                //获取每条curve的点的数量
                std::vector<unsigned int>  curve_points_count(curves_ptr->size());
                for(unsigned int i = 0; i < curves_ptr->size(); i++)
                {
                    curve_points_count[i] = (*curves_ptr)[i].get_size();
                    //std::cout << (*curves_ptr)[i].get_size() << std::endl;
                }

                unsigned int curves_points_sum = 0;//前面的curve一共有几个点
                unsigned int index = 0;//线的起始坐标
                for(unsigned int i = 0; i < curves_ptr->size(); i++){
                    for(unsigned int j = 0; j < (*curves_ptr)[i].get_size() - 1; j++){   
                        index = curves_points_sum + j + 1;
                        data_destination << "l" << " " << index << " " << (index + 1) << std::endl;
                    }
                    curves_points_sum = curves_points_sum + curve_points_count[i];
                }

                data_destination.close();
            }

            //保存一条曲线
            template <typename T>
            void save_curve_obj(const std::string file_name,wh::basic::adt::Curve<T>* curve_ptr){
                //打开文件
                std::ofstream data_destination(file_name);
                data_destination << "# whlib curve obj file" << std::endl;//文件头注释
                
                //获取当地时间
                time_t now = time(0);
                std::string date_time(ctime(&now));

                //注意时间后面自带换行
                data_destination << "# " << date_time;//写入存储时间
                data_destination << "o " << date_time;//以时间命名obj对象

                //存入数据
                //先存点
                const std::vector<T>& points = curve_ptr->get_points();
                for(unsigned int i = 0; i < curve_ptr->get_size(); i++){
                    data_destination << "v" << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[0];
                    data_destination << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[1];
                    data_destination << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[2] << std::endl;
                }
                //再存边
                for(unsigned int i = 0; i < curve_ptr->get_size()-1; i++){
                    data_destination << "l" << " " << i+1 << " " << (i+2) << std::endl;
                }
                data_destination.close();
            }

            //读取一条曲线
            template <typename T>
            void load_curve_obj(const std::string file_name,wh::basic::adt::Curve<T>* curve_ptr){
                std::ifstream data_source(file_name);
                if(!data_source.is_open()){
                    std::cerr<< "no data source." <<std::endl;
                }

                std::string line;
                std::vector<std::string> line_split;
                while(getline(data_source, line)){
                    line_split = wh::utils::split(line," ");
                    if(line_split[0] == "v"){
                        curve_ptr->get_points().push_back(
                            wh::basic::Point3d(std::stod(line_split[1]),std::stod(line_split[2]),std::stod(line_split[3]))
                        );
                        //curve_ptr->show();
                    }
                }
                curve_ptr->size = curve_ptr->get_points().size();
            }
        } 
    }
}
#endif