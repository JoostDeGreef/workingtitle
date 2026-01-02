
#include "internal/geometry/Transformation.h"

Transformation::Transformation()
    : transform(
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        })
{}

Transformation::Transformation(const Transformation& other)
    : transform(other.transform)
{}

Transformation::Transformation(Transformation && other) noexcept
    : transform(std::move(other.transform))
{}

Transformation::Transformation(const Vertex & translation)
    : transform(
        {
            1, 0, 0, translation.x,
            0, 1, 0, translation.y,
            0, 0, 1, translation.z,
            0, 0, 0, 1
        })
{}

Transformation::Transformation(const Normal & axis, const Scalar & angle)
{
    const auto n = axis.normalized();
    const auto& x = n.x;
    const auto& y = n.y;
    const auto& z = n.z;
    const auto c = cos(angle);
    const auto s = sin(angle);
    transform =
    { 
        x*x*(1-c)+1*c, y*x*(1-c)-z*s, z*x*(1-c)+y*s, 0,
        x*y*(1-c)+z*s, y*y*(1-c)+1*c, z*y*(1-c)-x*s, 0,
        x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+1*c, 0,
        0,             0,             0,             1
    };
}

Transformation::Transformation(const Normal& axis, const Normal& up)
{
    auto xAxis = Normal(up.crossProduct(axis));
    auto yAxis = Normal(axis.crossProduct(xAxis));
    transform =
    {
        xAxis.x, xAxis.y, xAxis.z, 0,
        yAxis.x, yAxis.y, yAxis.z, 0,
        axis.x,  axis.y,  axis.z,  0,
        0,       0,       0,       1
    };
}

Vertex Transformation::operator*(const Vertex & vertex) const
{
    Scalar x = transform[ 0] * vertex.x + transform[ 1] * vertex.y + transform[ 2] * vertex.z + transform[ 3];
    Scalar y = transform[ 4] * vertex.x + transform[ 5] * vertex.y + transform[ 6] * vertex.z + transform[ 7];
    Scalar z = transform[ 8] * vertex.x + transform[ 9] * vertex.y + transform[10] * vertex.z + transform[11];
    return { x, y, z };
}

Vertices Transformation::operator*(const Vertices& vertices) const
{
    Vertices res;
    res.reserve(vertices.size());
    for (const auto& vertex : vertices)
    {
        res.emplace_back(*this * vertex);
    }
    return res;
}

const Transformation Transformation::operator*(const Transformation& other) const
{
    return Transformation(*this)*=other;
}

Transformation& Transformation::operator*=(const Transformation& other)
{
    transform *= other.transform;
    return *this;
}

Transformation& Transformation::operator=(const Transformation& other)
{
    transform = other.transform;
    return *this;
}

