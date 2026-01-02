#pragma once

#include "internal/generic/Matrix.h"
#include "internal/generic/Normal.h"
#include "internal/generic/Vertex.h"
#include "internal/generic/Vertices.h"
#include "internal/generic/Scalar.h"

class Transformation
{
public:
    Transformation();
    Transformation(const Transformation & other);
    Transformation(Transformation && other) noexcept;
    Transformation(const Vertex & translation);
    Transformation(const Normal& axis, const Scalar& angle);
    Transformation(const Normal& axis, const Normal& up);

    Vertex operator * (const Vertex& v) const;
    Vertices operator * (const Vertices& v) const;

    const Transformation  operator *  (const Transformation& other) const;
          Transformation& operator *= (const Transformation& other);

    Transformation& operator = (const Transformation& other);
protected:
    Matrix<4,4,Scalar> transform;
};

