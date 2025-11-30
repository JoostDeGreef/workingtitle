#pragma once

#include "internal/geometry/BoundingObject.h"
#include "internal/geometry/Faces.h"
#include "internal/geometry/Vertices.h"

class Shape
{
public:
    Shape();
    Shape(const Shape & other);
    Shape(Shape && other) noexcept;

    // Scale (multiply) all vertices with this factor
    void scale(const double& factor);

    // Remove unused vertices
    void optimize();

private:
    // Vertices are stored in the shape:
    // - to save space (same vertex used in multiple faces)
    // - so transformations can be applied on all vertices at once
    Vertices vertices;

    Faces faces;

    BoundingObject bounds;
};

