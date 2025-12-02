#pragma once

#include "internal/generic/Constants.h"

#include "internal/geometry/BoundingBox.h"
#include "internal/geometry/IBoundingObject.h"
#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

class BoundingSphere final : public IBoundingObject 
{
public:
    Vertex center;
    Scalar radius;

    ~BoundingSphere();
    BoundingSphere() noexcept;
    BoundingSphere(
        const Vertex& center,
        const Scalar& radius);
    BoundingSphere(const Vertices& vertices);

    virtual void invalidate() override;
    virtual operator bool() const override;
    virtual Type getType() const override;
    virtual Scalar calculateVolume() const override;
    virtual void create(const Vertices& vertices) override;
    virtual void scale(const double& factor) override;
    virtual bool overlap(const IBoundingObject& other) const override;
            bool overlap(const BoundingSphere& other) const;
            bool overlap(const BoundingBox& other) const;
};

inline BoundingSphere::~BoundingSphere()
{};
inline BoundingSphere::BoundingSphere() noexcept
    : radius(Limits<Scalar>::MinValue)
{}
inline BoundingSphere::BoundingSphere(
    const Vertex& center,
    const Scalar& radius)
    : center(center)
    , radius(radius)
{}
inline BoundingSphere::BoundingSphere(const Vertices& vertices)
{
    create(vertices);
}

inline void BoundingSphere::invalidate()
{
    radius = Limits<Scalar>::MinValue;
}

inline BoundingSphere::operator bool() const
{
    return radius >= 0;
}

inline IBoundingObject::Type BoundingSphere::getType() const
{
    return Type::Sphere;
}

inline Scalar BoundingSphere::calculateVolume() const
{
    return 4 * radius * radius * radius * Constants::Pi / 3;
}

inline void BoundingSphere::create(const Vertices& vertices)
{
    // function to find the Vertex from the list furthest from the pivot
    auto findFurthestVertex = [](const Vertex &pivot, const Vertices& vertices)
        {
            Scalar dist2 = 0;
            Vertex p = pivot;
            for (const auto& Vertex : vertices)
            {
                auto tmpDist2 = Vertex.dist2(pivot); // radius^2
                if (tmpDist2 > dist2)
                {
                    p = Vertex;
                    dist2 = tmpDist2;
                }
            }
            return p;
        };
    // function to find the Vertex outside the circle and update the circle (center + radius^2)
    auto findOutliers = [](Vertex& center, Scalar radius2, const Vertices& vertices)
        {
            Vertices outliers;
            Scalar dist2Max = radius2;
            Vertex furthest = center;
            for (const auto& Vertex : vertices)
            {
                auto dist2 = Vertex.dist2(center);
                if (dist2 > radius2)
                {
                    outliers.emplace_back(Vertex);
                    if (dist2 > dist2Max)
                    {
                        dist2Max = dist2;
                        furthest = Vertex;
                    }
                }
            }
            if (!outliers.empty())
            {
                auto spoke = furthest - center;
                auto spokeLength = Numerics::Sqrt(spoke.innerProduct(spoke));
                auto diff = spokeLength - Numerics::Sqrt(radius2);
                center += spoke * (diff / (2 * spokeLength));
                radius2 = Numerics::Sqr(spokeLength - diff / 2);
            }
            return outliers;
        };
    // determine cloud center
    Vertex center;
    for (const auto& Vertex : vertices)
    {
        center += Vertex;
    }
    center /= (Scalar)vertices.size();
    // find Vertex p1 furthest from center
    auto p1 = findFurthestVertex(center, vertices);
    // find Vertex p2 furthest from p1 
    auto p2 = findFurthestVertex(p1, vertices);
    // calculate center and radius squared
    center = (p1 + p2) / 2;
    auto radius2 = p1.dist2(p2) / 4; // /4, this is radius^2
    // find vertices outside of initial sphere
    std::vector<Vertex> outliers = findOutliers(center, radius2, vertices);
    // increase bouding sphere untill there are no more outliers
    while (!outliers.empty())
    {
        outliers = findOutliers(center, radius2, outliers);
    }
    // normalize the radius
    radius = Numerics::Sqrt(radius2);
}

inline void BoundingSphere::scale(const double& factor)
{
    center *= factor;
    radius *= factor;
}

inline bool BoundingSphere::overlap(const IBoundingObject& other) const
{
    switch (other.getType())
    {
    case Type::Box:
        return overlap(*(BoundingBox*)(&other));
    case Type::Sphere:
        return overlap(*(BoundingSphere*)(&other));
    default:
        // maybe some other object can determine overlap with me?
        return other.overlap(*this);
    }
}

inline bool BoundingSphere::overlap(const BoundingSphere& other) const
{
    auto vec = center - other.center;
    return (vec.innerProduct(vec) <= Numerics::Sqr(radius + other.radius));
}

inline bool BoundingSphere::overlap(const BoundingBox& box) const
{
    if( center.x + radius >= box.min.x &&
        center.x - radius <= box.max.x &&
        center.y + radius >= box.min.y &&
        center.y - radius <= box.max.y &&
        center.z + radius >= box.min.z &&
        center.z - radius <= box.max.z)
    {
        Vertex corner = center;
        for (size_t i = 0; i < 3; ++i)
        {
            if (center[i] <= box.min[i]) corner[i] = box.min[i];
            else if (center[i] >= box.max[i]) corner[i] = box.max[i];
        }
        if(center.dist(corner) > radius)
        {
            return false;
        }
        return true;
    }
    return false;
}
