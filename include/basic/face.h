#ifndef FACE_H
#define FACE_H
#include <iostream>
#include <Eigen/Dense>
namespace wh
{
    namespace basic
    {
        typedef struct Face
        {
            unsigned int firVerIndex; //第一个顶点位置
            unsigned int secVerIndex; //第二个顶点位置
            unsigned int thdVerIndex; //第三个顶点位置
            Face();
            Face(Eigen::RowVector3i faceIndex);
            Face(unsigned int firVerIndex, unsigned int secVerIndex, unsigned int thdVerIndex);
            friend std::ostream &operator<<(std::ostream &ost, const wh::basic::Face face);
            bool operator<(const Face &face) const;
        } FACE;
        std::ostream &operator<<(std::ostream &ost, const wh::basic::Face face);
    } // namespace basic
} // namespace wh
#endif