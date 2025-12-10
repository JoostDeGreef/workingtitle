#pragma once

#include <vector>

#include "internal/generic/Numerics.h"
#include "internal/generic/Scalar.h"

//
// 2D Contours are defined counterclockwise in the xy plane.
//
class Contour2D
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

    Contour2D() 
        : points()
    {}
    Contour2D(const Contour2D & other)
        : points(other.points)
    {}
    Contour2D(Contour2D&& other) noexcept
        : points(std::move(other.points))
    {}
    Contour2D(const Points& points)
        : points(points)
    {}

    Contour2D& add(const Scalar& x, const Scalar& y)
    {
        points.emplace_back(x, y);
        return *this;
    }
    Contour2D& add(const Point& p)
    {
        points.emplace_back(p);
        return *this;
    }

    const Point& operator [](const size_t index) const { return points[index]; }

    auto size() const { return points.size(); }
    auto begin() const { return points.begin(); }
    auto end() const { return points.end(); }

    // predefined contours
    static Contour2D Square(const Point& min = Point(0, 0), const Point& max = Point(1, 1));
    static Contour2D Circle(const Point& center = Point(0, 0), const Scalar& radius = 1, const size_t & pieces = 10);

private:
    Points points;
};
