#pragma once

#include "internal/generic/Scalar.h"

struct Point
{
    Point()
        : x(0)
        , y(0)
    {
    }
    Point(const Scalar& x, const Scalar& y)
        : x(x)
        , y(y)
    {
    }

    Point operator + (const Point& other) const
    {
        return Point(x + other.x, y + other.y);
    }

    Scalar x;
    Scalar y;
};
