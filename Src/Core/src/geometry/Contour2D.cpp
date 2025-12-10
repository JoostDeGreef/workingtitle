

#include "internal/generic/Constants.h"

#include "internal/geometry/Contour2D.h"

Contour2D Contour2D::Square(const Point& min, const Point& max)
{
    return Contour2D()
        .add(min)
        .add(max.x,min.y)
        .add(max)
        .add(min.x,max.y);
}

Contour2D Contour2D::Circle(const Point& center, const Scalar& radius, const size_t & pieces)
{
    Contour2D res;
    for (size_t i = 0; i < pieces; ++i)
    {
        Scalar a = 2 * i * Constants::Pi / pieces;
        res.add(center.x + radius*cos(a), center.y + radius * sin(a));
    }
    return res;
}
