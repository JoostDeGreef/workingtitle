#pragma once

#include <algorithm>
#include <cassert>

#include "internal/generic/Index.h"

#include "internal/geometry/Face.h"
#include "internal/geometry/Normal.h"
#include "internal/geometry/Vertices.h"

class FaceVisitor
{
public:
    FaceVisitor(const Face & face, const Vertices & vertices);

    const Vertex& getVertex(const Index index) const;
    Normal calculateNormal() const;
    Scalar calculateSurface() const;
private:
    const Face& face;
    const Vertices& vertices;
};

inline FaceVisitor::FaceVisitor(const Face& face, const Vertices& vertices)
    : face(face)
    , vertices(vertices)
{}

inline const Vertex& FaceVisitor::getVertex(const Index index) const
{
    return vertices.at(face[index]);
}

inline Normal FaceVisitor::calculateNormal() const
{
    assert(face.count > 2); // degenerative face, no normal
    Vertex s0;
    Vertex s1 = getVertex(face.count - 2) - getVertex(face.count - 1);
    Normal n;
    for (Index i=0,j=face.count-1;i<face.count;j=i,++i)
    {
        s0 = s1;
        s1 = getVertex(j) - getVertex(i);
        n += s0.crossProduct(s1);
    }
    return n.normalize();
}

inline Scalar FaceVisitor::calculateSurface() const
{
    assert(face.count > 2); // degenerative face, no surface area
    Scalar area = 0;
    Vertex s0;
    Vertex s1 = getVertex(1) - getVertex(0);
    Normal n;
    for (Index i = 2; i < face.count; ++i)
    {
        s0 = s1;
        s1 = getVertex(i) - getVertex(i-1);
        area += s0.crossProduct(s1).length();
    }
    return area / 2;
}

