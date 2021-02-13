#include <basic/face.h>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
using namespace wh::basic;
using namespace std;

Face::Face() : firVerIndex(0), secVerIndex(0), thdVerIndex(0)
{
}

Face::Face(unsigned int firVerIndex, unsigned int secVerIndex, unsigned int thdVerIndex)
{
    vector<unsigned int> indexes(3);
    indexes[0] = firVerIndex;
    indexes[1] = secVerIndex;
    indexes[2] = thdVerIndex;
    sort(indexes.begin(), indexes.end());
    firVerIndex = indexes[0];
    secVerIndex = indexes[1];
    thdVerIndex = indexes[2];
}

Face::Face(Eigen::RowVector3i face_index)
{
    vector<unsigned int> indexes(3);
    indexes[0] = face_index[0];
    indexes[1] = face_index[1];
    indexes[2] = face_index[2];
    sort(indexes.begin(), indexes.end());
    firVerIndex = indexes[0];
    secVerIndex = indexes[1];
    thdVerIndex = indexes[2];
}

bool Face::operator<(const Face &face) const
{
    if (firVerIndex < face.firVerIndex)
    {
        return true;
    }
    else if (firVerIndex > face.firVerIndex)
    {
        return false;
    }

    if (secVerIndex < face.secVerIndex)
    {
        return true;
    }
    else if (secVerIndex > face.secVerIndex)
    {
        return false;
    }

    if (thdVerIndex < face.thdVerIndex)
    {
        return true;
    }
    else if (thdVerIndex > face.thdVerIndex)
    {
        return false;
    }
    return false;
}

std::ostream &wh::basic::operator<<(std::ostream &ost, const wh::basic::Face face)
{
    ost << face.firVerIndex << " " << face.secVerIndex << " " << face.thdVerIndex;
    return ost;
}