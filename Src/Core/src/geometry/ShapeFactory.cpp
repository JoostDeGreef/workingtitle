
#include "internal/geometry/Faces.h"
#include "internal/geometry/ShapeFactory.h"
#include "internal/geometry/Vertices.h"

Shape ShapeFactory::Box(const Vertex& min, const Vertex& max)
{
    Vertices vertices =
    {
        {max.x,max.y,max.z}, // 1
        {max.x,max.y,min.z}, // 2
        {min.x,max.y,min.z}, // 3
        {min.x,max.y,max.z}, // 4
        {max.x,min.y,max.z}, // 5
        {min.x,min.y,max.z}, // 6
        {min.x,min.y,min.z}, // 7
        {max.x,min.y,min.z}, // 8
    };
    Faces faces =
    {
        Face({0,3,2,1}),
        Face({0,1,7,4}),
        Face({0,4,5,3}),
        Face({1,2,6,7}),
        Face({2,3,5,6}),
        Face({4,7,6,5})
    };
    return Shape(vertices, faces);
}

Shape ShapeFactory::Dodecahedron(const Vertex& center, const Scalar& radius)
{
    // See doc/Dodecahedron.md

    const Scalar s = (1 + sqrt(5)) / 2;
    const Scalar t = 1 / s;

    Vertices vertices =
    {
        {-1, -1, -1}, // 0
        {-1,  1, -1}, // 1
        {-1, -1,  1}, // 2
        {-1,  1,  1}, // 3
        { 1, -1, -1}, // 4
        { 1,  1, -1}, // 5
        { 1, -1,  1}, // 6
        { 1,  1,  1}, // 7

        { 0, -s, -t}, // 8
        { 0,  s, -t}, // 9
        { 0, -s,  t}, // 10
        { 0,  s,  t}, // 11

        {-t,  0, -s}, // 12
        { t,  0, -s}, // 13
        {-t,  0,  s}, // 14
        { t,  0,  s}, // 15

        {-s, -t,  0}, // 16
        {-s,  t,  0}, // 15
        { s, -t,  0}, // 18 
        { s,  t,  0}, // 19
        
    };
    Faces faces =
    {
        Face({0,  8,  4, 13, 12}), // 0
        Face({0, 12,  1, 17, 16}), // 1
        Face({0, 16,  2, 10,  8}), // 2
        Face({1,  9, 11,  3, 17}), // 3
        Face({1, 12, 13,  5,  9}), // 4
        Face({2, 14, 15,  6, 10}), // 5
        Face({2, 16, 17,  3, 14}), // 6
        Face({3, 11,  7, 15, 14}), // 7
        Face({4,  8, 10,  6, 18}), // 8
        Face({4, 18, 19,  5, 13}), // 9
        Face({5, 19,  7, 11,  9}), // 10
        Face({6, 15,  7, 19, 18}), // 11
    };

    for (auto & v : vertices)
    {
        v *= radius/v.length();
        v += center;
    }

    return Shape(vertices, faces);
}

Shape ShapeFactory::Octahedron(const Vertex& center, const Scalar& radius)
{
    Vertices vertices =
    {
        { 0,  0, -1}, // 0
        { 0, -1,  0}, // 1
        {-1,  0,  0}, // 2
        { 0,  1,  0}, // 3
        { 1,  0,  0}, // 4
        { 0,  0,  1}, // 5
    };
    Faces faces =
    {
        Face({0,1,4}),
        Face({0,2,1}),
        Face({0,3,2}),
        Face({0,4,3}),
        Face({1,5,4}),
        Face({1,2,5}),
        Face({2,3,5}),
        Face({3,4,5})
    };
    return Shape(vertices, faces);
}

Shape ShapeFactory::Extrusion(const Contour& contour, const Scalar& height)
{
    Index s = (Index)contour.size();
    Vertices vertices;
    vertices.resize(s * 2);
    Faces faces;
    faces.reserve(s + 2);
    std::vector<Index> bottom,top;
    bottom.reserve(s);
    top.reserve(s);
    Index j = s - 1;
    for (Index i = 0; i < s; ++i)
    {
        const auto& p = contour[i];
        vertices[i] = Vertex(p.x, p.y, 0);
        vertices[s+i] = Vertex(p.x, p.y, height);
        faces.push_back(Face({ i, j, (Index)(s + j), (Index)(s + i) }));
        bottom.emplace_back(i);
        top.emplace_back(2*s-i-1);
        j = i;
    }
    faces.push_back(Face(bottom));
    faces.push_back(Face(top));
    return Shape(vertices, faces);
}
