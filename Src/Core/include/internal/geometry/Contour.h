#pragma once

#include <vector>

#include "internal/generic/Numerics.h"
#include "internal/generic/Scalar.h"

//
// Contours are defined counterclockwise in the xy plane.
//
class Contour
{
public:
    struct Point
    {
        Point()
            : x(0)
            , y(0)
        {}
        Point(const Scalar &x, const Scalar &y)
            : x(x)
            , y(y)
        {}

        Point operator + (const Point & other) const 
        {
            return Point(x + other.x, y + other.y);
        }

        Scalar x;
        Scalar y;
    };
    typedef std::vector<Point> Points;

    Contour() 
        : points()
    {}
    Contour(const Contour & other)
        : points(other.points)
    {}
    Contour(Contour&& other) noexcept
        : points(std::move(other.points))
    {}
    Contour(const Points& points)
        : points(points)
    {}

    Contour& add(const Scalar& x, const Scalar& y)
    {
        points.emplace_back(x, y);
        return *this;
    }
    Contour& add(const Point& p)
    {
        points.emplace_back(p);
        return *this;
    }

    const Point& operator [](const size_t index) const { return points[index]; }

    auto size() const { return points.size(); }
    auto begin() const { return points.begin(); }
    auto end() const { return points.end(); }

    // predefined contours
    static Contour Square(const Point& min = Point(0, 0), const Point& max = Point(1, 1));
    static Contour Circle(const Point& center = Point(0, 0), const Scalar& radius = 1, const size_t & pieces = 10);

private:
    Points points;
};
