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
        s1 = getVertex(i) - getVertex(j);
        n += s1.crossProduct(s0);
    }
    return n.normalize();
}

inline Scalar FaceVisitor::calculateSurface() const
{
    assert(face.count > 2); // degenerative face, no surface area
    Vertex area(0,0,0);
    Vertex s0;
    const Vertex& v0 = getVertex(0);
    Vertex s1 = getVertex(1) - v0;
    for (Index i = 2; i < face.count; ++i)
    {
        s0 = s1;
        s1 = getVertex(i) - v0;
        area += s0.crossProduct(s1);
    }

    return area.length() / 2;
}

