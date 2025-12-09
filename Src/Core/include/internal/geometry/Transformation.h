#pragma once

#include <array>

#include "internal/generic/Scalar.h"

#include "internal/geometry/Normal.h"
#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

class Transformation
{
public:
    Transformation();
    Transformation(const Transformation & other);
    Transformation(Transformation && other) noexcept;
    Transformation(const Vertex & translation);
    Transformation(const Normal& axis, const Scalar& angle);

    Vertex operator * (const Vertex& v) const;
    Vertices operator * (const Vertices& v) const;

    Transformation& operator *= (const Transformation& other);
protected:
    std::array<Scalar, 12> transform;
};

