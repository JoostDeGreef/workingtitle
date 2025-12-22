#pragma once

#include "internal/generic/Vertex.h"

class Normal : public Vertex
{
public:
    using Vertex::Vertex;

    Normal(const Vertex & vertex);

    Normal & normalize();
    Normal normalized() const;
};

inline Normal::Normal(const Vertex& vertex)
{
    x = vertex.x;
    y = vertex.y;
    z = vertex.z;
    normalize();
}

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

