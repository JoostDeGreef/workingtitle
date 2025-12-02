#pragma once

#include "internal/geometry/Vertex.h"

class Normal : public Vertex
{
    using Vertex::Vertex;
public:

    Normal & normalize();
};

inline Normal & Normal::normalize()
{
    Scalar l = length();
    *this /= l;
    return *this;
}

