#include "internal/geometry/Shape.h"

Shape::Shape()
    : faces()
    , vertices()
    , bounds()
{}

Shape::Shape(const Shape& other)
    : faces(other.faces)
    , vertices(other.vertices)
    , bounds(other.bounds)
{}

Shape::Shape(Shape&& other) noexcept
    : faces(std::move(other.faces))
    , vertices(std::move(other.vertices))
    , bounds(std::move(other.bounds))
{}

void Shape::scale(const double& factor)
{
    for (auto& vertex : vertices)
    {
        vertex *= factor;
    }
    bounds.scale(factor);
}

void Shape::optimize()
{
    std::vector<size_t> counts(vertices.size(), 0);
    for (const auto& face : faces)
    {
        for (const auto& point : face)
        {
            ++counts[point];
        }
    }
    size_t j = 0;
    std::vector<Index> mapping(vertices.size(), 0);
    for (size_t i = 0; i < counts.size(); ++i)
    {
        if (counts[i] != 0)
        {
            vertices[i].swap(vertices[j]);
            mapping[i] = (Index)j++;
        }
    }
    vertices.resize(j);
    for (auto& face : faces)
    {
        for (auto& point : face)
        {
            point = mapping[point];
        }
    }
}
