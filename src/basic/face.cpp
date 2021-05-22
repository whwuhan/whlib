#include <basic/face.h>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
using namespace wh::basic;
using namespace std;

Face::Face() : fir_ver_index(0), sec_ver_index(0), thd_ver_index(0){}

Face::Face(unsigned int fir_ver_index, unsigned int sec_ver_index, unsigned int thd_ver_index)
{
    vector<unsigned int> indices(3);
    indices[0] = fir_ver_index;
    indices[1] = sec_ver_index;
    indices[2] = thd_ver_index;
    sort(indices.begin(), indices.end());
    fir_ver_index = indices[0];
    sec_ver_index = indices[1];
    thd_ver_index = indices[2];
}

Face::Face(Eigen::RowVector3i face_index)
{
    vector<unsigned int> indices(3);
    indices[0] = face_index[0];
    indices[1] = face_index[1];
    indices[2] = face_index[2];
    sort(indices.begin(), indices.end());
    fir_ver_index = indices[0];
    sec_ver_index = indices[1];
    thd_ver_index = indices[2];
}

bool Face::operator<(const Face &face) const
{
    if (fir_ver_index < face.fir_ver_index)
    {
        return true;
    }
    else if (fir_ver_index > face.fir_ver_index)
    {
        return false;
    }

    if (sec_ver_index < face.sec_ver_index)
    {
        return true;
    }
    else if (sec_ver_index > face.sec_ver_index)
    {
        return false;
    }

    if (thd_ver_index < face.thd_ver_index)
    {
        return true;
    }
    else if (thd_ver_index > face.thd_ver_index)
    {
        return false;
    }
    return false;
}

std::ostream &wh::basic::operator<<(std::ostream &ost, const wh::basic::Face face)
{
    ost << face.fir_ver_index << " " << face.sec_ver_index << " " << face.thd_ver_index;
    return ost;
}