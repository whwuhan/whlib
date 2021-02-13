#ifndef EDGE_H
#define EDGE_H
#include <iostream>
namespace wh
{
    namespace basic
    {
        typedef struct Edge
        {
            //无序边默认第一个顶点一定比第二个顶点位置小
            unsigned int firVerIndex; //第一个顶点位置
            unsigned int secVerIndex; //第二个顶点位置

            Edge();
            Edge(unsigned int firVerIndex, unsigned int secVerIndex, bool unordered = true);
            friend std::ostream &operator<<(std::ostream &ost, const wh::basic::Edge edge);
            bool operator<(const Edge &edge) const;
        } EDGE;
        std::ostream &operator<<(std::ostream &ost, const wh::basic::Edge edge);
    } // namespace basic
} // namespace wh
#endif