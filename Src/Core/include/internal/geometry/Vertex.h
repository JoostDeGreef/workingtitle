#pragma once

template<typename COORDINATE>
class Vertex
{
public:
    COORDINATE x;
    COORDINATE y;
    COORDINATE z;

    Vertex() 
        : x(0)
        , y(0)
        , z(0)
    {}
    Vertex(
        const COORDINATE x,
        const COORDINATE y,
        const COORDINATE z)
        : x(x)
        , y(y)
        , z(z)
    {}
    Vertex(
        const Vertex<COORDINATE> & other)
    {
        copy(other);
    }
    Vertex(
        Vertex<COORDINATE> && other)
    {
        swap(other);
    }

    Vertex<COORDINATE>& operator = (const Vertex<COORDINATE>& other)
    {
        copy(other);
        return *this;
    }
    Vertex<COORDINATE>& operator = (Vertex<COORDINATE>&& other)
    {
        swap(other);
        return *this;
    }

    void copy(const Vertex<COORDINATE>& other);
    void swap(Vertex<COORDINATE>& other);

    Vertex<COORDINATE>   operator + (const Vertex<COORDINATE>& other) const;
    Vertex<COORDINATE> & operator += (const Vertex<COORDINATE>& other);

    Vertex<COORDINATE>   operator - (const Vertex<COORDINATE>& other) const;
    Vertex<COORDINATE> & operator -= (const Vertex<COORDINATE>& other);

    Vertex<COORDINATE>   operator * (const COORDINATE& factor) const;
    Vertex<COORDINATE> & operator *= (const COORDINATE& factor);

    Vertex<COORDINATE>   operator / (const COORDINATE& factor) const;
    Vertex<COORDINATE> & operator /= (const COORDINATE& factor);

    bool operator == (const Vertex<COORDINATE>& other) const;
    bool operator != (const Vertex<COORDINATE>& other) const;

    COORDINATE innerProduct(const Vertex<COORDINATE>& other) const;
};

template<typename COORDINATE>
inline void Vertex<COORDINATE>::copy(const Vertex<COORDINATE>& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

template<typename COORDINATE>
inline void Vertex<COORDINATE>::swap(Vertex<COORDINATE>& other)
{
    std::swap(x, other.x);
    std::swap(y, other.y);
    std::swap(z, other.z);
}

template<typename COORDINATE>
inline Vertex<COORDINATE> Vertex<COORDINATE>::operator + (const Vertex<COORDINATE>& other) const
{
    return Vertex<COORDINATE>(*this)+=other;
}

template<typename COORDINATE>
inline Vertex<COORDINATE> & Vertex<COORDINATE>::operator += (const Vertex<COORDINATE>& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

template<typename COORDINATE>
inline Vertex<COORDINATE> Vertex<COORDINATE>::operator - (const Vertex<COORDINATE>& other) const
{
    return Vertex<COORDINATE>(*this) -= other;
}

template<typename COORDINATE>
inline Vertex<COORDINATE>& Vertex<COORDINATE>::operator -= (const Vertex<COORDINATE>& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

template<typename COORDINATE>
inline Vertex<COORDINATE> Vertex<COORDINATE>::operator * (const COORDINATE& factor) const
{
    return Vertex<COORDINATE>(*this) *= factor;
}

template<typename COORDINATE>
inline Vertex<COORDINATE>& Vertex<COORDINATE>::operator *= (const COORDINATE& factor)
{
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
}

template<typename COORDINATE>
inline Vertex<COORDINATE> Vertex<COORDINATE>::operator/(const COORDINATE& factor) const
{
    return Vertex<COORDINATE>(*this) /= factor;
}

template<typename COORDINATE>
inline Vertex<COORDINATE>& Vertex<COORDINATE>::operator/=(const COORDINATE& factor)
{
    x /= factor;
    y /= factor;
    z /= factor;
    return *this;
}

template<typename COORDINATE>
inline bool Vertex<COORDINATE>::operator == (const Vertex<COORDINATE>& other) const
{
    return Numerics::Equal(x, other.x)
        && Numerics::Equal(y, other.y)
        && Numerics::Equal(z, other.z);
}

template<typename COORDINATE>
inline bool Vertex<COORDINATE>::operator != (const Vertex<COORDINATE>& other) const
{
    return Numerics::NotEqual(x, other.x)
        || Numerics::NotEqual(y, other.y)
        || Numerics::NotEqual(z, other.z);
}

template<typename COORDINATE>
inline COORDINATE Vertex<COORDINATE>::innerProduct(const Vertex<COORDINATE>& other) const
{
    return x * other.x + y * other.y + z * other.z;
}
