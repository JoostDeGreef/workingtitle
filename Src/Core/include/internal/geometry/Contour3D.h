#pragma once

#include "internal/generic/Scalar.h"

#include "internal/geometry/Vertices.h"

//
// 3D Contours are defined counterclockwise in the xyz plane.
//
class Contour3D
{
public:
    Contour3D() 
        : vertices()
    {}
    Contour3D(const Contour3D & other)
        : vertices(other.vertices)
    {}
    Contour3D(Contour3D&& other) noexcept
        : vertices(std::move(other.vertices))
    {}
    Contour3D(const Vertices& vertices)
        : vertices(vertices)
    {}

    Contour3D& add(const Scalar& x, const Scalar& y, const Scalar & z)
    {
        vertices.emplace_back(x, y, z);
        return *this;
    }
    Contour3D& add(const Vertex& v)
    {
        vertices.emplace_back(v);
        return *this;
    }

    const Vertex& operator [](const size_t index) const { return vertices[index]; }

    auto size() const { return vertices.size(); }
    auto begin() const { return vertices.begin(); }
    auto end() const { return vertices.end(); }

private:
    Vertices vertices;
};
