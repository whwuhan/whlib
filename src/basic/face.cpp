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
    vector<unsigned int> indices(3);
    indices[0] = firVerIndex;
    indices[1] = secVerIndex;
    indices[2] = thdVerIndex;
    sort(indices.begin(), indices.end());
    firVerIndex = indices[0];
    secVerIndex = indices[1];
    thdVerIndex = indices[2];
}

Face::Face(Eigen::RowVector3i faceIndex)
{
    vector<unsigned int> indices(3);
    indices[0] = faceIndex[0];
    indices[1] = faceIndex[1];
    indices[2] = faceIndex[2];
    sort(indices.begin(), indices.end());
    firVerIndex = indices[0];
    secVerIndex = indices[1];
    thdVerIndex = indices[2];
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