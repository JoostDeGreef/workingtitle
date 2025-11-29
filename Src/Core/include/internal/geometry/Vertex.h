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
        const Vertex<COORDINATE>& other);
    Vertex(
        Vertex<COORDINATE>&& other) noexcept;

    Vertex<COORDINATE>& operator = (const Vertex<COORDINATE>& other);
    Vertex<COORDINATE>& operator = (Vertex<COORDINATE>&& other) noexcept;
    COORDINATE& operator [] (const size_t index);
    COORDINATE operator [] (const size_t index) const;

    void copy(const Vertex<COORDINATE>& other);
    void swap(Vertex<COORDINATE>& other) noexcept;

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
    COORDINATE dist2(const Vertex<COORDINATE>& other) const;
    COORDINATE dist(const Vertex<COORDINATE>& other) const;
};

template<typename COORDINATE>
inline Vertex<COORDINATE>::Vertex(const Vertex<COORDINATE>& other)
{
    copy(other);
}

template<typename COORDINATE>
inline Vertex<COORDINATE>::Vertex(Vertex<COORDINATE>&& other) noexcept
{
    swap(other);
}

template<typename COORDINATE>
inline Vertex<COORDINATE>& Vertex<COORDINATE>::operator=(const Vertex<COORDINATE>& other)
{
    copy(other);
    return *this;
}

template<typename COORDINATE>
inline Vertex<COORDINATE>& Vertex<COORDINATE>::operator=(Vertex<COORDINATE>&& other) noexcept
{
    swap(other);
    return *this;
}

template<typename COORDINATE>
inline COORDINATE& Vertex<COORDINATE>::operator[](const size_t index)
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    }
#ifndef NDEBUG
    throw invalid_argument("index out of bounds");
#endif // NDEBUG
}

template<typename COORDINATE>
inline COORDINATE Vertex<COORDINATE>::operator[](const size_t index) const
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    }
#ifndef NDEBUG
    throw invalid_argument("index out of bounds");
#endif // NDEBUG
}

template<typename COORDINATE>
inline void Vertex<COORDINATE>::copy(const Vertex<COORDINATE>& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

template<typename COORDINATE>
inline void Vertex<COORDINATE>::swap(Vertex<COORDINATE>& other) noexcept
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

template<typename COORDINATE>
inline COORDINATE Vertex<COORDINATE>::dist2(const Vertex<COORDINATE>& other) const
{
    auto tmp = *this - other;
    return tmp.innerProduct(tmp);
}

template<typename COORDINATE>
inline COORDINATE Vertex<COORDINATE>::dist(const Vertex<COORDINATE>& other) const
{
    return Numerics::Sqrt(dist2(other));
}





