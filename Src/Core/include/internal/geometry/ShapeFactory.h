#pragma once

#include "internal/generic/Scalar.h"

#include "internal/geometry/Contour.h"
#include "internal/geometry/Shape.h"
#include "internal/geometry/Vertex.h"

class ShapeFactory
{
public:
    static Shape Box(const Vertex& min = Vertex(0, 0, 0), const Vertex& max = Vertex(1, 1, 1));
    static Shape Dodecahedron(const Vertex& center = Vertex(0, 0, 0), const Scalar& radius = 1);
    static Shape Extrusion(const Contour& contour = Contour::Square(), const Scalar& height = 1);
    static Shape Octahedron(const Vertex& center = Vertex(0, 0, 0), const Scalar& radius = 1);
};

