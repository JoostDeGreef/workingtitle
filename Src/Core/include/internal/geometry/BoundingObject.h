#pragma once

#include <cassert>
#include <variant>

#include "internal/geometry/BoundingBox.h"
#include "internal/geometry/BoundingSphere.h"
#include "internal/geometry/IBoundingObject.h"
#include "internal/geometry/Vertex.h"
#include "internal/geometry/Vertices.h"

class BoundingObject final : public IBoundingObject
{
public:
    ~BoundingObject();
    BoundingObject() noexcept;
    BoundingObject(
        const Vertex& min,
        const Vertex& max);
    BoundingObject(
        const Vertex& center,
        const Scalar& radius);
    BoundingObject(const BoundingObject& other);
    BoundingObject(BoundingObject&& other) noexcept;
    BoundingObject(const Vertices& vertices);
    BoundingObject(const Vertices& vertices, const Type type);

    BoundingObject& operator = (const BoundingObject& other);
    BoundingObject& operator = (BoundingObject&& other) noexcept;

    void copy(const BoundingObject& other);
    void swap(BoundingObject& other);

    virtual void invalidate() override;
    virtual operator bool() const override;
    virtual Type getType() const override;
    virtual Scalar calculateVolume() const override;
    virtual void create(const Vertices& vertices) override;
            void create(const Vertices& vertices, const Type type);
    virtual void scale(const double& factor) override;
    virtual bool overlap(const IBoundingObject& other) const override;

private:
    std::variant<BoundingBox, BoundingSphere> object;
};

inline BoundingObject::~BoundingObject()
{}

inline BoundingObject::BoundingObject() noexcept
    : object(BoundingSphere())
{}

inline BoundingObject::BoundingObject(const Vertex & min, const Vertex & max)
    : object(BoundingBox(min,max))
{}

inline BoundingObject::BoundingObject(const Vertex& center, const Scalar& radius)
    : object(BoundingSphere(center,radius))
{}

inline BoundingObject::BoundingObject(const BoundingObject& other)
{
    copy(other);
}

inline BoundingObject::BoundingObject(BoundingObject&& other) noexcept
    : object(other.object)
{
    swap(other);
}

inline BoundingObject::BoundingObject(const Vertices& vertices)
{
    create(vertices);
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
    object = other.object;
}

inline void BoundingObject::swap(BoundingObject& other)
{
    std::swap(object,other.object);
}

inline void BoundingObject::create(const Vertices& vertices)
{
    auto box = BoundingBox(vertices);
    auto sphere = BoundingSphere(vertices);
    auto boxVolume = box.calculateVolume();
    auto sphereVolume = sphere.calculateVolume();
    if (boxVolume <= sphereVolume)
    {
        object = box;
    }
    else
    {
        object = sphere;
    }
}

inline void BoundingObject::create(const Vertices& vertices, const Type type)
{
    switch (type)
    {
    case Type::Box:
        object = BoundingBox(vertices);
        break;
    case Type::Sphere:
        object = BoundingSphere(vertices);
        break;
    default:
        assert(false); // unhandled type
    }
}

inline typename BoundingObject::Type BoundingObject::getType() const
{
    return std::visit([](auto&& arg) { return arg.getType(); }, object);
}

inline Scalar BoundingObject::calculateVolume() const
{
    return std::visit([](auto&& arg) { return arg.calculateVolume(); }, object);
}

inline bool BoundingObject::overlap(const IBoundingObject& other) const
{
    return std::visit([&other](auto&& arg) { return arg.overlap(other); }, object);
}

inline void BoundingObject::scale(const double& factor)
{
    return std::visit([&factor](auto&& arg) { return arg.scale(factor); }, object);
}

inline void BoundingObject::invalidate()
{
    return std::visit([](auto&& arg) { return arg.invalidate(); }, object);
}

inline BoundingObject::operator bool() const
{
    return std::visit([](auto&& arg) { return (bool)arg; }, object);
}

