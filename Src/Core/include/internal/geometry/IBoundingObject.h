#pragma once

#include "internal/generic/Vertices.h"

class IBoundingObject
{
public:
    enum class Type
    {
        Sphere = 1,
        Box = 2,
    };

    virtual ~IBoundingObject() {}
    virtual void invalidate() = 0;
    virtual operator bool() const = 0;
    virtual Type getType() const = 0;
    virtual Scalar calculateVolume() const = 0;
    virtual void create(const Vertices& vertices) = 0;
    virtual void scale(const double& factor) = 0;
    virtual bool overlap(const IBoundingObject& other) const = 0;
};

