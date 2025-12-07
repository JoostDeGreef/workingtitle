
#include "internal/geometry/Transformation.h"

Transformation::Transformation()
    : transform(
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0
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
            0, 0, 1, translation.z
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
        x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+1*c, 1
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

Transformation& Transformation::operator*=(const Transformation& other)
{
    Scalar m0  = transform[ 0] * other.transform[ 0] + transform[ 1] * other.transform[ 4] + transform[ 2] * other.transform[ 8];
    Scalar m1  = transform[ 0] * other.transform[ 1] + transform[ 1] * other.transform[ 5] + transform[ 2] * other.transform[ 9];
    Scalar m2  = transform[ 0] * other.transform[ 2] + transform[ 1] * other.transform[ 6] + transform[ 2] * other.transform[10];
    Scalar m3  = transform[ 0] * other.transform[ 3] + transform[ 1] * other.transform[ 7] + transform[ 2] * other.transform[11] + transform[ 3];

    Scalar m4  = transform[ 4] * other.transform[ 0] + transform[ 5] * other.transform[ 4] + transform[ 6] * other.transform[ 8];
    Scalar m5  = transform[ 4] * other.transform[ 1] + transform[ 5] * other.transform[ 5] + transform[ 6] * other.transform[ 9];
    Scalar m6  = transform[ 4] * other.transform[ 2] + transform[ 5] * other.transform[ 6] + transform[ 6] * other.transform[10];
    Scalar m7  = transform[ 4] * other.transform[ 3] + transform[ 5] * other.transform[ 7] + transform[ 6] * other.transform[11] + transform[ 7];

    Scalar m8  = transform[ 8] * other.transform[ 0] + transform[ 9] * other.transform[ 4] + transform[10] * other.transform[ 8];
    Scalar m9  = transform[ 8] * other.transform[ 1] + transform[ 9] * other.transform[ 5] + transform[10] * other.transform[ 9];
    Scalar m10 = transform[ 8] * other.transform[ 2] + transform[ 9] * other.transform[ 6] + transform[10] * other.transform[10];
    Scalar m11 = transform[ 8] * other.transform[ 3] + transform[ 9] * other.transform[ 7] + transform[10] * other.transform[11] + transform[11];

    transform = {m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11};

    return *this;
}

