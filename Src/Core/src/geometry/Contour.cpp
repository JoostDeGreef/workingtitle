

#include "internal/generic/Constants.h"

#include "internal/geometry/Contour.h"

Contour Contour::Square(const Point& min, const Point& max)
{
    return Contour()
        .add(min)
        .add(max.x,min.y)
        .add(max)
        .add(min.x,max.y);
}

Contour Contour::Circle(const Point& center, const Scalar& radius, const size_t & pieces)
{
    Contour res;
    for (size_t i = 0; i < pieces; ++i)
    {
        Scalar a = 2 * i * Constants::Pi / pieces;
        res.add(center.x + radius*cos(a), center.y + radius * sin(a));
    }
    return res;
}
