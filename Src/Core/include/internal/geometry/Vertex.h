#pragma once

#include <stdexcept>

#include "internal/generic/Numerics.h"
#include "internal/generic/Scalar.h"

class Vertex
{
public:
    Scalar x;
    Scalar y;
    Scalar z;

    Vertex() 
        : x(0)
        , y(0)
        , z(0)
    {}
    Vertex(
        const Scalar x,
        const Scalar y,
        const Scalar z)
        : x(x)
        , y(y)
        , z(z)
    {}
    Vertex(
        const Vertex& other);
    Vertex(
        Vertex&& other) noexcept;

    Vertex& operator = (const Vertex& other);
    Vertex& operator = (Vertex&& other) noexcept;
    Scalar& operator [] (const size_t index);
    Scalar operator [] (const size_t index) const;

    void copy(const Vertex& other);
    void swap(Vertex& other) noexcept;

    Vertex   operator + (const Vertex& other) const;
    Vertex & operator += (const Vertex& other);

    Vertex   operator - (const Vertex& other) const;
    Vertex & operator -= (const Vertex& other);

    Vertex   operator * (const Scalar& factor) const;
    Vertex & operator *= (const Scalar& factor);

    Vertex   operator / (const Scalar& factor) const;
    Vertex & operator /= (const Scalar& factor);

    bool operator == (const Vertex& other) const;
    bool operator != (const Vertex& other) const;

    Scalar innerProduct(const Vertex& other) const;
    Scalar dist2(const Vertex& other) const;
    Scalar dist(const Vertex& other) const;
};

inline Vertex::Vertex(const Vertex& other)
{
    copy(other);
}

inline Vertex::Vertex(Vertex&& other) noexcept
{
    swap(other);
}

inline Vertex& Vertex::operator=(const Vertex& other)
{
    copy(other);
    return *this;
}

inline Vertex& Vertex::operator=(Vertex&& other) noexcept
{
    swap(other);
    return *this;
}

inline Scalar& Vertex::operator[](const size_t index)
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    }
#ifndef NDEBUG
    throw std::invalid_argument("index out of bounds");
#endif // NDEBUG
}

inline Scalar Vertex::operator[](const size_t index) const
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    }
#ifndef NDEBUG
    throw std::invalid_argument("index out of bounds");
#endif // NDEBUG
}

inline void Vertex::copy(const Vertex& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

inline void Vertex::swap(Vertex& other) noexcept
{
    std::swap(x, other.x);
    std::swap(y, other.y);
    std::swap(z, other.z);
}

inline Vertex Vertex::operator + (const Vertex& other) const
{
    return Vertex(*this)+=other;
}

inline Vertex & Vertex::operator += (const Vertex& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

inline Vertex Vertex::operator - (const Vertex& other) const
{
    return Vertex(*this) -= other;
}

inline Vertex& Vertex::operator -= (const Vertex& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

inline Vertex Vertex::operator * (const Scalar& factor) const
{
    return Vertex(*this) *= factor;
}

inline Vertex& Vertex::operator *= (const Scalar& factor)
{
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
}

inline Vertex Vertex::operator/(const Scalar& factor) const
{
    return Vertex(*this) /= factor;
}

inline Vertex& Vertex::operator/=(const Scalar& factor)
{
    x /= factor;
    y /= factor;
    z /= factor;
    return *this;
}

inline bool Vertex::operator == (const Vertex& other) const
{
    return Numerics::Equal(x, other.x)
        && Numerics::Equal(y, other.y)
        && Numerics::Equal(z, other.z);
}

inline bool Vertex::operator != (const Vertex& other) const
{
    return Numerics::NotEqual(x, other.x)
        || Numerics::NotEqual(y, other.y)
        || Numerics::NotEqual(z, other.z);
}

inline Scalar Vertex::innerProduct(const Vertex& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

inline Scalar Vertex::dist2(const Vertex& other) const
{
    auto tmp = *this - other;
    return tmp.innerProduct(tmp);
}

inline Scalar Vertex::dist(const Vertex& other) const
{
    return Numerics::Sqrt(dist2(other));
}





