#pragma once

#include <cassert>
#include <vector>

#include "internal/generic/Index.h"

class Face
{
public:
    static constexpr size_t maxFixPoints = sizeof(Index*) / sizeof(Index);

    class iterator {
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
    private:
        iterator(Index* points, Index index)
            : points(points)
            , index(index)
        {}

        Index* points;
        Index index;
    };
    class const_iterator {
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

    void swap(Face& other);

    Index size() const;

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

inline void Face::swap(Face& other)
{
    std::swap(count,other.count);
    std::swap(varPoints,other.varPoints);
}

inline Index Face::size() const
{
    return count;
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
