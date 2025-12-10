#pragma once

#include "internal/geometry/Face.h"
#include "internal/geometry/Normal.h"
#include "internal/geometry/Vertices.h"

class FaceVisitor
{
public:
    FaceVisitor(const Face & face, const Vertices & vertices, const Normal & normal)
        : face(face)
        , vertices(vertices)
        , normal(normal)
    {}

    const Normal& getNormal() const
    {
        return normal;
    }

    Index size() const 
    { 
        return face.size();
    }

    const Vertex& operator [] (const Index index) const
    {
        return vertices.at(face[index]);
    }

    class const_iterator
    {
    public:
        friend class FaceVisitor;

        const_iterator()
            : face(nullptr)
            , index(0)
        {}
        const_iterator(const const_iterator& other)
            : face(other.face)
            , index(other.index)
        {}
        ~const_iterator()
        {}

        const_iterator& operator=(const const_iterator& other)
        {
            face = other.face;
            index = other.index;
        }
        bool operator==(const const_iterator& other) const
        {
            assert(face == other.face); // comparing iterators from different faces
            return index == other.index;
        }
        bool operator!=(const const_iterator& other) const
        {
            assert(face == other.face); // comparing iterators from different faces
            return index != other.index;
        }

        const_iterator& operator++()
        {
            ++index;
            return *this;
        }
        const_iterator& operator--()
        {
            --index;
            return *this;
        }
        const Vertex& operator*() const
        {
            return face->operator[](index);
        }
    private:
        const_iterator(const FaceVisitor* face, const Index index)
            : face(face)
            , index(index)
        {}

        const FaceVisitor* face;
        Index index;
    };

    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator end() const;
    const_iterator cend() const;

private:
    const Face& face;
    const Vertices& vertices;
    const Normal& normal;
};

inline FaceVisitor::const_iterator FaceVisitor::begin() const
{
    return cbegin();
}
inline FaceVisitor::const_iterator FaceVisitor::cbegin() const
{
    return const_iterator(this, 0);
}

inline FaceVisitor::const_iterator FaceVisitor::end() const
{
    return cend();
}
inline FaceVisitor::const_iterator FaceVisitor::cend() const
{
    return const_iterator(this, face.size());
}
