#include "Core.h"

Shape::Shape()
    : faces()
    , vertices()
{}

Shape::Shape(const Shape& other)
    : faces(other.faces)
    , vertices(other.vertices)
{}

Shape::Shape(Shape&& other) noexcept
    : faces(std::move(other.faces))
    , vertices(std::move(other.vertices))
{}

void Shape::Scale(const double& factor)
{
    for (auto& vertex : vertices)
    {
        vertex *= factor;
    }
}

void Shape::Optimize()
{
    std::vector<size_t> counts(vertices.size(), 0);
    for (const auto& face : faces)
    {
        ++counts[face.points[0]];
        ++counts[face.points[1]];
        ++counts[face.points[2]];
    }
    size_t j = 0;
    std::vector<index> mapping(vertices.size(), 0);
    for (size_t i = 0; i < counts.size(); ++i)
    {
        if (counts[i] != 0)
        {
            vertices[i].swap(vertices[j]);
            mapping[i] = j++;
        }
    }
    vertices.resize(j);
    for (auto& face : faces)
    {
        face.points[0] = mapping[face.points[0]];
        face.points[1] = mapping[face.points[1]];
        face.points[2] = mapping[face.points[2]];
    }
}
