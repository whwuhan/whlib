#ifndef IO_SKEL_H
#define IO_SKEL_H
#include <string>
#include <iomanip>
#include <fstream>
#include <basic/curve.h>
#include <utils/string_lib.h>
namespace wh
{
    namespace utils
    {
        namespace io
        {
            //======声明======
            //读取skel文件中的骨架线
            template <typename T>
            void loadCurvesSkel(const std::string fileName, std::vector<wh::basic::Curve<T> > *curvesPtr);

            //=====实现=======
            //读取skel文件中的骨架线
            template <typename T>
            void loadCurvesSkel(const std::string fileName, std::vector<wh::basic::Curve<T> > *curvesPtr)
            {
                //打开文件
                std::ifstream dataSrc(fileName);
                if (!dataSrc.is_open())
                {
                    std::cout << "no data source." << std::endl;
                    return;
                }
                //读入文件
                std::string line;
                std::vector<std::string> lineSplit;
                unsigned int curvesCount = 0; //curves的数量
                unsigned int pointsCount = 0; //每条curve中点的数量
                while (getline(dataSrc, line))
                {
                    if ("CN" == line.substr(0, 2) && "CNN" != line.substr(0, 3))
                    {
                        lineSplit = wh::utils::split(line, " ");
                        curvesCount = atoi(lineSplit[1].c_str());
                        curvesPtr->resize(curvesCount);
                        curvesCount = 0; //作为index
                    }

                    if ("CNN" == line.substr(0, 3))
                    {
                        //确定curve中的点数量
                        lineSplit = wh::utils::split(line, "   ");
                        pointsCount = atoi(lineSplit[1].c_str());

                        //存储单独的一条边中的点
                        std::vector<T> curve(pointsCount);
                        for (int i = 0; i < pointsCount; i++)
                        {
                            if (getline(dataSrc, line))
                            {                                              //往下读一行
                                lineSplit = wh::utils::split(line, "\t"); //注意skel文件不是用的空格，用的制表符
                                double x = atof(lineSplit[0].c_str());
                                double y = atof(lineSplit[1].c_str());
                                double z = atof(lineSplit[2].c_str());
                                curve[i] = T(x, y, z); //一条边中的点
                            }
                            else
                            {
                                std::cout << "wrong skel data." << std::endl;
                            }
                        }
                        //将curve放入curves
                        (*curvesPtr)[curvesCount++] = wh::basic::Curve<T>(curve);
                    }
                }
                dataSrc.close();
            }
        } // namespace io
    } // namespace utils
} // namespace wh
#endif