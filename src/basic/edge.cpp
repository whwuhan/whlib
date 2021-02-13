#include <basic/edge.h>
using namespace wh::basic;
using namespace std;
Edge::Edge() : firVerIndex(0), secVerIndex(0)
{
}

Edge::Edge(unsigned int firVerIndex, unsigned int secVerIndex, bool unordered)
{
    //unordered:是否无序
    if (unordered)
    {
        if (firVerIndex < secVerIndex)
        {
            this->firVerIndex = firVerIndex;
            this->secVerIndex = secVerIndex;
        }
        else
        {
            this->firVerIndex = secVerIndex;
            this->secVerIndex = firVerIndex;
        }
    }
    else
    {
        this->firVerIndex = firVerIndex;
        this->secVerIndex = secVerIndex;
    }
}

bool Edge::operator<(const Edge &edge) const
{
    if (firVerIndex < edge.firVerIndex)
    {
        return true;
    }
    else if (firVerIndex == edge.firVerIndex)
    {
        if (secVerIndex < edge.secVerIndex)
            return true;
    }
    return false;
}