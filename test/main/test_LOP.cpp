#include <iostream>
#include <basic/cube.h>
#include <utils/io/io_obj.h>
#include <basic/octree.h>
#include <algorithm/LOP.h>
#include <utils/string_lib.h>
using namespace std;
using namespace wh::basic;
using namespace wh::utils::io;
using namespace wh::alg;
using namespace Eigen;
int main(){
    LOP lop;
    lop.setFileName("Horse.obj");
    lop.parameters.iter = 10;
    lop.parameters.amount = 1000;
    lop.init();
    lop.run();
}