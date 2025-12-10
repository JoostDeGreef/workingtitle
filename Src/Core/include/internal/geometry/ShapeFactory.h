#pragma once

#include "internal/generic/Scalar.h"

#include "internal/geometry/Contour2D.h"
#include "internal/geometry/Shape.h"
#include "internal/geometry/Vertex.h"

class ShapeFactory
{
public:
    static Shape Box(const Vertex& min = Vertex(0, 0, 0), const Vertex& max = Vertex(1, 1, 1));
    static Shape Dodecahedron(const Vertex& center = Vertex(0, 0, 0), const Scalar& radius = 1);
    static Shape Octahedron(const Vertex& center = Vertex(0, 0, 0), const Scalar& radius = 1);

    // contour should be defined counterclockwise.
    static Shape Extrusion(const Contour2D& contour = Contour2D::Square(), const Scalar& height = 1);
};

