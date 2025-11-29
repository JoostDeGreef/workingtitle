#pragma once

class Shape
{
public:
    typedef size_t index;
    typedef Triangle<index> triangle;
    typedef Vertex<double> vertex;

    Shape();
    Shape(const Shape & other);
    Shape(Shape && other) noexcept;

    void Scale(const double& factor);

    void Optimize();

private:
    std::vector<triangle> faces;
    std::vector<vertex> vertices;
};

