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
#include <basic/graph.h>
#include <basic/curve.h>
#include <basic/polygon_mesh.h>
#include <utils/string_lib.h>
#include <utils/io/io_obj.h>
namespace wh
{
    namespace utils
    {
        namespace io
        {
            //======声明=========
            //点云PointCloud
            void loadPointCloudObj(const std::string fileName, struct wh::basic::PointCloud *pointCloudPtr);
            void savePointCloudObj(const std::string fileName, const struct wh::basic::PointCloud *const pointCloudPtr);

            //曲线curve
            //保存一条曲线
            template <typename T>
            void saveCurveObj(const std::string fileName, std::vector<wh::basic::Curve<T>> *curvesPtr);
            //保存一系列曲线
            template <typename T>
            void saveCurvesObj(const std::string fileName, const std::vector<wh::basic::Curve<T>> *const curvesPtr);
            //读取一条曲线
            template <typename T>
            void loadCurveObj(const std::string fileName, wh::basic::Curve<T> *curvePtr);

            //存储立方体cube mesh
            //存储为三角网格模型
            void saveTriCubeMeshObj(const std::string fileName, wh::basic::Cube *cubePtr);
            void saveTriCubeMeshesObj(const std::string fileName, const std::vector<wh::basic::Cube> &cubes);
            void saveTriCubeMeshesObj(const std::string fileName, const std::set<wh::basic::Cube> &cubes);
            void saveCubeWireframeObj(const std::string fileName, wh::basic::Cube *cubePtr);
            void saveCubeWireframesObj(const std::string fileName, const std::vector<wh::basic::Cube> &cubes);
            void saveCubeWireframesObj(const std::string fileName, const std::set<wh::basic::Cube> &cubes);

            //polygon mesh
            void loadPolygonMeshObj(const std::string fileName, wh::basic::PolygonMesh *polygonMeshPtr);
            void savePolygonMeshObj(const std::string fileName, wh::basic::PolygonMesh *polygonMeshPtr);

            //skeleton 骨架线
            void loadSkeletonObj(const std::string fileName, wh::basic::Skeleton *skeletonPtr);
            void saveSkeletonObj(const std::string fileName, wh::basic::Skeleton *skeletonPtr);












            //=========模板方法实现=========
            //保存一条曲线
            template <typename T>
            void saveCurveObj(const std::string fileName, wh::basic::Curve<T> *curvePtr)
            {
                //打开文件
                std::ofstream dataDes(fileName);
                dataDes << "# whlib curve obj file" << std::endl; //文件头注释

                //获取当地时间
                time_t now = time(0);
                std::string dateTime(ctime(&now));

                //注意时间后面自带换行
                dataDes << "# " << dateTime;                                                //写入存储时间
                std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "./");         //obj对象命名
                dataDes << "o " << fileNameSplit[fileNameSplit.size() - 2] << std::endl; //obj对象命名

                //存入数据
                //先存点
                const std::vector<T> &points = curvePtr->getPoints();
                for (unsigned int i = 0; i < curvePtr->getSize(); i++)
                {
                    dataDes << "v"
                                     << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[0];
                    dataDes << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[1];
                    dataDes << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[2] << std::endl;
                }
                //再存边
                for (unsigned int i = 0; i < curvePtr->getSize() - 1; i++)
                {
                    dataDes << "l"
                                     << " " << i + 1 << " " << (i + 2) << std::endl;
                }
                dataDes.close();
            }




            // 保存一系列曲线
            template <typename T>
            void saveCurvesObj(const std::string fileName, std::vector<wh::basic::Curve<T>> *const curvesPtr)
            {
                //打开文件
                std::ofstream dataDes(fileName);
                dataDes << "# whlib skeleton obj file" << std::endl; //文件头注释

                //获取当地时间
                time_t now = time(0);
                std::string dateTime(ctime(&now));

                //注意时间后面自带换行
                dataDes << "# " << dateTime;                                                //写入存储时间
                std::vector<std::string> fileNameSplit = wh::utils::split(fileName, "./");         //obj对象命名
                dataDes << "o " << fileNameSplit[fileNameSplit.size() - 2] << std::endl; //obj对象命名

                //存入数据
                //先存点
                for (auto it = curvesPtr->begin(); it != curvesPtr->end(); it++)
                {
                    const std::vector<T> &points = it->getPoints();
                    for (unsigned int i = 0; i < it->getSize(); i++)
                    {
                        dataDes << "v"
                                         << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[0];
                        dataDes << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[1];
                        dataDes << " " << std::setiosflags(std::ios::fixed) << std::setprecision(10) << points[i].data[2] << std::endl;
                    }
                }
                //再存边
                //获取每条curve的点的数量
                std::vector<unsigned int> curvePointsCount(curvesPtr->size());
                for (unsigned int i = 0; i < curvesPtr->size(); i++)
                {
                    curvePointsCount[i] = (*curvesPtr)[i].getSize();
                    //std::cout << (*curvesPtr)[i].getSize() << std::endl;
                }

                unsigned int curvesPointsSum = 0; //前面的curve一共有几个点
                unsigned int index = 0;             //线的起始坐标
                for (unsigned int i = 0; i < curvesPtr->size(); i++)
                {
                    for (unsigned int j = 0; j < (*curvesPtr)[i].getSize() - 1; j++)
                    {
                        index = curvesPointsSum + j + 1;
                        dataDes << "l"
                                         << " " << index << " " << (index + 1) << std::endl;
                    }
                    curvesPointsSum = curvesPointsSum + curvePointsCount[i];
                }
                std::cout << "Save Curves Successfully" << std::endl;
                dataDes.close();
            }






            //读取一条曲线
            template <typename T>
            void loadCurveObj(const std::string fileName, wh::basic::Curve<T> *curvePtr)
            {
                std::ifstream dataSrc(fileName);
                if (!dataSrc.is_open())
                {
                    std::cerr << "no data source." << std::endl;
                }

                std::string line;
                std::vector<std::string> lineSplit;
                while (getline(dataSrc, line))
                {
                    lineSplit = wh::utils::split(line, " ");
                    if (lineSplit[0] == "v")
                    {
                        curvePtr->getPoints().push_back(
                            wh::basic::Point3d(std::stod(lineSplit[1]), std::stod(lineSplit[2]), std::stod(lineSplit[3])));
                        //curvePtr->show();
                    }
                }
                curvePtr->size = curvePtr->getPoints().size();
            }
        }// namespace io
    }// namespace utils
}// namespace wh
#endif