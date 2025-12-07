#pragma once

#include "internal/geometry/Vertex.h"

class Normal : public Vertex
{
    using Vertex::Vertex;
public:

    Normal & normalize();
    Normal normalized() const;
};

inline Normal & Normal::normalize()
{
    Scalar l = length();
    *this /= l;
    return *this;
}

inline Normal Normal::normalized() const
{
    return Normal(*this).normalize();
}

