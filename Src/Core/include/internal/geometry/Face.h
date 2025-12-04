#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "internal/generic/Index.h"

class FaceVisitor;

class Face
{
public:
    friend class FaceVisitor;
        
    static constexpr size_t maxFixPoints = sizeof(Index*) / sizeof(Index);

    class iterator 
    {
    public:
        friend class Face;

        iterator()
            : points(nullptr)
            , index(0)
        {}
        iterator(const iterator& other)
            : points(other.points)
            , index(other.index)
        {}
        ~iterator()
        {}

        iterator& operator=(const iterator& other)
        {
            points = other.points;
            index = other.index;
        }
        bool operator==(const iterator& other) const
        {
            assert(points == other.points); // comparing iterators from different Face instances
            return index == other.index;
        }
        bool operator!=(const iterator& other) const
        {
            assert(points == other.points); // comparing iterators from different Face instances
            return index != other.index;
        }

        iterator& operator++()
        {
            ++index;
            return *this;
        }
        Index & operator*() const
        {
            return points[index];
        }

        void swap(iterator& other)
        {
            assert(points == other.points); // swapping iterators from different Face instances
            std::swap(points, other.points);
            std::swap(index, other.index);
        }
    private:
        iterator(Index* points, Index index)
            : points(points)
            , index(index)
        {}

        Index* points;
        Index index;
    };
    class const_iterator 
    {
    public:
        friend class Face;

        const_iterator()
            : points(nullptr)
            , index(0)
        {}
        const_iterator(const const_iterator& other)
            : points(other.points)
            , index(other.index)
        {}
        const_iterator(const iterator& other)
            : points(other.points)
            , index(other.index)
        {}
        ~const_iterator()
        {}

        const_iterator& operator=(const const_iterator& other)
        {
            points = other.points;
            index = other.index;
        }
        const_iterator& operator=(const iterator& other)
        {
            points = other.points;
            index = other.index;
        }
        bool operator==(const const_iterator& other) const
        {
            assert(points == other.points); // comparing iterators from different Face instances
            return index == other.index;
        }
        bool operator!=(const const_iterator& other) const
        {
            assert(points == other.points); // comparing iterators from different Face instances
            return index != other.index;
        }

        const_iterator& operator++()
        {
            ++index;
            return *this;
        }
        const Index& operator*() const
        {
            return points[index];
        }
    private:
        const_iterator(Index* points, Index index)
            : points(points)
            , index(index)
        {
        }

        Index* points;
        Index index;
    };

    Face()
        : count(0)
    {}
    Face(const Face & other)
        : count(0)
    {
        copy(other);
    }
    Face(Face&& other) noexcept
        : count(0)
    {
        swap(other);
    }
    Face(const std::vector<Index>& points)
        : count(0)
    {
        set(points);
    }
    ~Face()
    {
        clear();
    }

    void set(const std::vector<Index>& points);
    void copy(const Face& other);
    void swap(Face& other);
    void clear();

    Index size() const;

    const Index& operator [] (const Index index) const;
          Index& operator [] (const Index index);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
private:
    Index  count;
    union
    {
        Index* varPoints;
        Index  fixPoints[maxFixPoints];
    };
};

inline void Face::clear()
{
    if (count > maxFixPoints)
    {
        delete[] varPoints;
    }
    count = 0;
}
inline void Face::set(const std::vector<Index>& points)
{
    clear();
    count = points.size();
    if (count <= maxFixPoints)
    {
        for (size_t i = 0; i < count; ++i)
        {
            fixPoints[i] = points[i];
        }
    }
    else
    {
        varPoints = new Index[count];
        for (size_t i = 0; i < count; ++i)
        {
            varPoints[i] = points[i];
        }
    }
}
inline void Face::copy(const Face& other)
{
    clear();
    count = other.count;
    if (count <= maxFixPoints)
    {
        std::copy_n(other.fixPoints, count, fixPoints);
    }
    else
    {
        varPoints = new Index[count];
        std::copy_n(other.varPoints, count, varPoints);
    }
}

inline void Face::swap(Face& other)
{
    std::swap(count,other.count);
    std::swap(varPoints,other.varPoints); // swaps all fixPoints or varPoints ptr 
}

inline Index Face::size() const
{
    return count;
}

inline const Index& Face::operator[](const Index index) const
{
    assert(index < count);
    if (count <= maxFixPoints)
    {
        return fixPoints[index];
    }
    else
    {
        return varPoints[index];
    }
}
inline Index& Face::operator[](const Index index)
{
    assert(index < count);
    if (count <= maxFixPoints)
    {
        return fixPoints[index];
    }
    else
    {
        return varPoints[index];
    }
}

inline Face::iterator Face::begin()
{
    if (count <= maxFixPoints)
        return iterator((Index *) &fixPoints, 0);
    else
        return iterator(varPoints, 0);
}
inline Face::const_iterator Face::begin() const
{
    return cbegin();
}
inline Face::const_iterator Face::cbegin() const
{
    if (count <= maxFixPoints)
        return const_iterator((Index*)&fixPoints, 0);
    else
        return const_iterator(varPoints, 0);
}

inline Face::iterator Face::end()
{
    if (count <= maxFixPoints)
        return iterator((Index*)&fixPoints, count);
    else
        return iterator(varPoints, count);
}
inline Face::const_iterator Face::end() const
{
    return cend();
}
inline Face::const_iterator Face::cend() const
{
    if (count <= maxFixPoints)
        return const_iterator((Index*)&fixPoints, count);
    else
        return const_iterator(varPoints, count);
}
