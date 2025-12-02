#pragma once

#include <algorithm>

#include "internal/geometry/IBoundingObject.h"
#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

class BoundingBox final : public IBoundingObject
{
public:
    Vertex min;
    Vertex max;

    ~BoundingBox();
    BoundingBox() noexcept;
    BoundingBox(
        const Vertex& min,
        const Vertex& max);
    BoundingBox(const Vertices& vertices);

    virtual void invalidate() override;
    virtual operator bool() const override;
    virtual Type getType() const override;
    virtual Scalar calculateVolume() const override;
    virtual void create(const Vertices& vertices) override;
    virtual void scale(const double& factor) override;
    virtual bool overlap(const IBoundingObject& other) const override;
            bool overlap(const BoundingBox& other) const;
};

inline BoundingBox::~BoundingBox() 
{};
inline BoundingBox::BoundingBox() noexcept
{
    invalidate();
}
inline BoundingBox::BoundingBox(
    const Vertex& min,
    const Vertex& max)
    : min(min)
    , max(max)
{}
inline BoundingBox::BoundingBox(const Vertices& vertices)
{
    create(vertices);
}

inline void BoundingBox::invalidate()
{
    max.x = Limits<Scalar>::MinValue;
    max.y = Limits<Scalar>::MaxValue;
}
inline BoundingBox::operator bool() const
{
    return max.x >= min.x;
}
inline IBoundingObject::Type BoundingBox::getType() const
{
    return Type::Box;
}
inline Scalar BoundingBox::calculateVolume() const
{
    return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

inline void BoundingBox::create(const Vertices& vertices)
{
    min = Vertex(Limits<Scalar>::MaxValue, Limits<Scalar>::MaxValue, Limits<Scalar>::MaxValue);
    max = Vertex(Limits<Scalar>::MinValue, Limits<Scalar>::MinValue, Limits<Scalar>::MinValue);
    for (const auto& Vertex : vertices)
    {
        min.x = std::min(min.x, Vertex.x);
        min.y = std::min(min.y, Vertex.y);
        min.z = std::min(min.z, Vertex.z);
        max.x = std::max(max.x, Vertex.x);
        max.y = std::max(max.y, Vertex.y);
        max.z = std::max(max.z, Vertex.z);
    }
}

inline void BoundingBox::scale(const double& factor)
{
    min *= factor;
    max *= factor;
}

inline bool BoundingBox::overlap(const BoundingBox& other) const
{
    return
        max.x >= other.min.x &&
        min.x <= other.max.x &&
        max.y >= other.min.y &&
        min.y <= other.max.y &&
        max.z >= other.min.z &&
        min.z <= other.max.z;
}

inline bool BoundingBox::overlap(const IBoundingObject& other) const
{
    switch (other.getType())
    {
    case Type::Box:
        return overlap(*(BoundingBox*)(&other));
    default:
        // maybe some other object can determine overlap with me?
        return other.overlap(*this);
    }
}



