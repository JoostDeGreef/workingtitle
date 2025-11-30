#pragma once

#include <algorithm>
#include <cassert>

#include "internal/generic/Constants.h"

#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

class BoundingObject
{
public:
    enum class Type
    {
        None = 0,
        Sphere = 1,
        Box = 2,
        Auto = 3,
    };
    struct Box
    {
        Vertex min;
        Vertex max;
    };
    struct Sphere
    {
        Vertex center;
        Scalar radius;
    };

    BoundingObject();
    BoundingObject(
        const Vertex& min,
        const Vertex& max);
    BoundingObject(
        const Vertex& center,
        const Scalar& radius);
    BoundingObject(const BoundingObject& other);
    BoundingObject(BoundingObject&& other) noexcept;
    BoundingObject(const Vertices& vertices, const Type type = Type::Auto);

    BoundingObject& operator = (const BoundingObject& other);
    BoundingObject& operator = (BoundingObject&& other) noexcept;

    void copy(const BoundingObject& other);
    void swap(BoundingObject& other);

    void create(const Vertices& vertices, const Type type);

    Type getType() const;

    Scalar calculateVolume() const;

    bool overlap(const BoundingObject &other) const;

    void scale(const double& factor);

    void invalidate();
    operator bool() const;

private:
    void createBox(const Vertices& vertices);
    void createSphere(const Vertices& vertices);

    Type type;
    union
    {
        Box box;
        Sphere sphere;
    };
};

inline BoundingObject::BoundingObject()
    : type(Type::None)
{}

inline BoundingObject::BoundingObject(const Vertex & min, const Vertex & max)
    : type(Type::Box)
{
    box.min = min;
    box.max = max;
}

inline BoundingObject::BoundingObject(const Vertex& center, const Scalar& radius)
    : type(Type::Sphere)
{
    sphere.center = center;
    sphere.radius = radius;
}

inline BoundingObject::BoundingObject(const BoundingObject& other)
{
    copy(other);
}

inline BoundingObject::BoundingObject(BoundingObject&& other) noexcept
    : type(Type::None)
{
    swap(other);
}

inline BoundingObject::BoundingObject(const Vertices& vertices, const Type type)
{
    create(vertices, type);
}

inline BoundingObject& BoundingObject::operator=(const BoundingObject& other)
{
    copy(other);
    return *this;
}

inline BoundingObject& BoundingObject::operator=(BoundingObject&& other) noexcept
{
    swap(other);
    return *this;
}

inline void BoundingObject::copy(const BoundingObject& other)
{
    type = other.type;
    switch (type)
    {
    case Type::Auto:
    case Type::None:
        break;
    case Type::Box:
        box = other.box;
        break;
    case Type::Sphere:
        sphere = other.sphere;
        break;
    default:
        assert(false); // unhandled type
    }
}

inline void BoundingObject::swap(BoundingObject& other)
{
    switch (type)
    {
    case Type::Auto:
    case Type::None:
        copy(other);
        other.type = Type::None;
        break;
    case Type::Box:
        {
            Box tmp = box;
            copy(other);
            other.type = Type::Box;
            other.box = tmp;
        }
        break;
    case Type::Sphere:
        {
            Sphere tmp = sphere;
            copy(other);
            other.type = Type::Sphere;
            other.sphere = tmp;
        }
        break;
    default:
        assert(false); // unhandled type
    }
}

inline void BoundingObject::create(const Vertices& vertices, const Type type)
{
    switch (type)
    {
    case Type::None:
        this->type = Type::None;
        break;
    case Type::Auto:
        {
            auto boBox = BoundingObject(vertices, Type::Box);
            auto boSphere = BoundingObject(vertices, Type::Sphere);
            auto boBoxVolume = boBox.calculateVolume();
            auto boSphereVolume = boSphere.calculateVolume();
            if ( boBoxVolume <= boSphereVolume )
            {
                copy(boBox);
            }
            else
            {
                copy(boSphere);
            }
        }
        break;
    case Type::Box:
        createBox(vertices);
        break;
    case Type::Sphere:
        createSphere(vertices);
        break;
    default:
        assert(false); // unhandled type
    }
}

inline typename BoundingObject::Type BoundingObject::getType() const
{
    return type;
}

inline Scalar BoundingObject::calculateVolume() const
{
    switch (type)
    {
    case Type::Box:
        return (box.max.x-box.min.x) * (box.max.y - box.min.y) * (box.max.z - box.min.z);
    case Type::Sphere:
        return 4 * sphere.radius * sphere.radius * sphere.radius * Constants::Pi / 3;
    case Type::None:
    case Type::Auto:
        return Scalar(-1);
    default:
        assert(false); // unhandled type
    }
}

inline bool BoundingObject::overlap(const BoundingObject& other) const
{
    if (type == Type::Box)
    {
        if (other.type == Type::Box)
        {
            return
                box.max.x >= other.box.min.x &&
                box.min.x <= other.box.max.x &&
                box.max.y >= other.box.min.y &&
                box.min.y <= other.box.max.y &&
                box.max.z >= other.box.min.z &&
                box.min.z <= other.box.max.z;
        }
        else if (other.type == Type::Sphere)
        {
            return other.overlap(*this);
        }
    }
    else if (type == Type::Sphere)
    {
        if (other.type == Type::Box)
        {
            if( sphere.center.x + sphere.radius >= other.box.min.x &&
                sphere.center.x - sphere.radius <= other.box.max.x &&
                sphere.center.y + sphere.radius >= other.box.min.y &&
                sphere.center.y - sphere.radius <= other.box.max.y &&
                sphere.center.z + sphere.radius >= other.box.min.z &&
                sphere.center.z - sphere.radius <= other.box.max.z)
            {
                Vertex corner = sphere.center;
                for (size_t i = 0; i < 3; ++i)
                {
                    if (sphere.center[i] <= other.box.min[i]) corner[i] = other.box.min[i];
                    else if (sphere.center[i] >= other.box.max[i]) corner[i] = other.box.max[i];
                }
                if(sphere.center.dist(corner) > sphere.radius)
                {
                    return false;
                }
                return true;
            }
            return false;
        }
        else if (other.type == Type::Sphere)
        {
            auto vec = sphere.center - other.sphere.center;
            return (vec.innerProduct(vec) <= Numerics::Sqr(sphere.radius + other.sphere.radius));
        }
    }
    return false;
}

inline void BoundingObject::scale(const double& factor)
{
    switch (type)
    {
    case Type::Box:
        box.min *= factor;
        box.max *= factor;
        break;
    case Type::Sphere:
        sphere.center *= factor;
        sphere.radius *= factor;
        break;
    case Type::None:
    case Type::Auto:
        break;
    default:
        assert(false); // unhandled type
        break;
    }
}

inline void BoundingObject::invalidate()
{
    type = Type::None;
}

inline BoundingObject::operator bool() const
{
    return type == Type::Box || type == Type::Sphere;
}

inline void BoundingObject::createBox(const Vertices& vertices)
{
    type = Type::Box;
    box.min = Vertex(Limits<Scalar>::MaxValue, Limits<Scalar>::MaxValue, Limits<Scalar>::MaxValue);
    box.max = Vertex(Limits<Scalar>::MinValue, Limits<Scalar>::MinValue, Limits<Scalar>::MinValue);
    for (const auto& Vertex : vertices)
    {
        box.min.x = std::min(box.min.x, Vertex.x);
        box.min.y = std::min(box.min.y, Vertex.y);
        box.min.z = std::min(box.min.z, Vertex.z);
        box.max.x = std::max(box.max.x, Vertex.x);
        box.max.y = std::max(box.max.y, Vertex.y);
        box.max.z = std::max(box.max.z, Vertex.z);
    }
}

inline void BoundingObject::createSphere(const Vertices& vertices)
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
    // store the results
    type = Type::Sphere;
    sphere.center = center;
    sphere.radius = Numerics::Sqrt(radius2);
}
