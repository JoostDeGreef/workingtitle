#pragma once

#include <algorithm>
#include <cassert>

#include "internal/generic/Index.h"

#include "internal/geometry/Faces.h"
#include "internal/geometry/Shape.h"
#include "internal/geometry/Vertices.h"

class ShapeFactory
{
public:
    static Shape Cube(const Vertex& min = Vertex(0, 0, 0), const Vertex& max = Vertex(1, 1, 1));
};

inline Shape ShapeFactory::Cube(const Vertex& min, const Vertex& max)
{
    Vertices vertices = 
    { 
        {max.x,max.y,max.z},
        {max.x,max.y,min.z},
        {min.x,max.y,min.z},
        {min.x,max.y,max.z},
        {max.x,min.y,max.z},
        {min.x,min.y,max.z},
        {min.x,min.y,min.z},
        {max.x,min.y,min.z},
    };
    Faces faces = 
    {
        Face({0,1,2,3}),
        Face({4,5,6,7}),
        Face({0,4,7,1}),
        Face({1,7,6,2}),
        Face({2,6,5,3}),
        Face({0,3,5,4})
    };
    Shape cube(vertices, faces);
    return cube;
}
