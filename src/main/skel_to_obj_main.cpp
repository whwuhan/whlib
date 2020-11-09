#include <iostream>
#include <fstream>
#include <vector>
#include <utils/io/io_obj.h>
#include <utils/io/io_skel.h>
#include <utils/string_lib.h>
#include <basic/abstract_data_type/curve.h>
#include <basic/point3d.h>
using namespace wh::utils;
using namespace wh::basic;
using namespace std;
int main(){
    ifstream file_list("/Users/wuhan/wuhan/CodingSpace/MultilevelDividedSkeletonExtraction/ExperimentData/skel/file_list.txt");
    if(!file_list.is_open()){
        cout << "no file_list." << endl;
        return 0;
    }
    string line;
    vector<string> line_split;
    while(getline(file_list, line)){
        line_split = split(line, ".");
        cout<<line_split[0]<<endl;
        if("skel" == line_split[1]){
            vector< adt::Curve<Point3d> > curves;
            io::load_curves_skel("/Users/wuhan/wuhan/CodingSpace/MultilevelDividedSkeletonExtraction/ExperimentData/skel/" + line, &curves);
            io::save_curves_obj("/Users/wuhan/wuhan/CodingSpace/MultilevelDividedSkeletonExtraction/ExperimentData/skel_obj/" + line_split[0] + ".obj", &curves);
        }
    }
    file_list.close();
}