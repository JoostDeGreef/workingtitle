

#include "internal/geometry/Contour.h"

Contour Contour::Square(const Point& min, const Point& max)
{
    return Contour()
        .add(min)
        .add(max.x,min.y)
        .add(max)
        .add(min.x,max.y);
}

