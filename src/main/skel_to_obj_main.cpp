#include <iostream>
#include <fstream>
#include <vector>
#include <utils/io/io_obj.h>
#include <utils/io/io_skel.h>
#include <utils/string_lib.h>
#include <basic/curve.h>
#include <basic/point3d.h>
using namespace wh::utils;
using namespace wh::basic;
using namespace std;
int main()
{
    ifstream fileList("C:\\Users\\Administrator\\Desktop\\Alan\\skel\\FileList.txt");
    if (!fileList.is_open())
    {
        cout << "no fileList." << endl;
        return 0;
    }
    string line;
    vector<string> lineSplit;
    while (getline(fileList, line))
    {
        lineSplit = split(line, ".");
        cout << lineSplit[0] << endl;
        if ("skel" == lineSplit[1])
        {
            vector<Curve<Point3d> > curves;
            io::loadCurvesSkel("C:\\Users\\Administrator\\Desktop\\Alan\\skel\\" + line, &curves);
            io::saveCurvesObj("C:\\Users\\Administrator\\Desktop\\Alan\\skel\\" + lineSplit[0] + ".obj", &curves);
        }
    }
    fileList.close();
}