#pragma once

template<typename VERTEX_INDEX>
class Triangle
{
public:
    VERTEX_INDEX points[3];

    void swap(Triangle<VERTEX_INDEX>& other);
};

template<typename VERTEX_INDEX>
inline void Triangle<VERTEX_INDEX>::swap(Triangle<VERTEX_INDEX>& other)
{
    std::swap(points[0], other.points[0]);
    std::swap(points[1], other.points[1]);
    std::swap(points[2], other.points[2]);
}
