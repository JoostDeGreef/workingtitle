#pragma once

#include "internal/generic/Vertices.h"
#include "internal/generic/Normals.h"

#include "internal/geometry/Faces.h"
#include "internal/geometry/FaceVisitor.h"

class FacesVisitor
{
public:

    class const_iterator
    {
    public:
        friend class FacesVisitor;

        ~const_iterator()
        {
        }

        bool operator==(const const_iterator& other) const
        {
            assert(&faces == &other.faces); // comparing iterators from different Shape instances
            return index == other.index;
        }
        bool operator!=(const const_iterator& other) const
        {
            assert(&faces == &other.faces); // comparing iterators from different Shape instances
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
        FaceVisitor operator*() const
        {
            return FaceVisitor(faces.at(index), vertices, normals.at(index));
        }
    private:
        const_iterator(const Faces& faces, const Vertices& vertices, const Normals& normals, Index index)
            : faces(faces)
            , vertices(vertices)
            , normals(normals)
            , index(index)
        {
        }

        const Faces& faces;
        const Vertices& vertices;
        const Normals& normals;
        Index index;
    };

    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator end() const;
    const_iterator cend() const;

    FacesVisitor(const Faces& faces, const Vertices& vertices, const Normals& normals)
        : faces(faces)
        , vertices(vertices)
        , normals(normals)
    {}
private:
    const Faces& faces;
    const Vertices& vertices;
    const Normals& normals;
};

inline FacesVisitor::const_iterator FacesVisitor::begin() const
{
    return cbegin();
}
inline FacesVisitor::const_iterator FacesVisitor::cbegin() const
{
    return const_iterator(faces, vertices, normals, 0);
}
inline FacesVisitor::const_iterator FacesVisitor::end() const
{
    return cend();
}
inline FacesVisitor::const_iterator FacesVisitor::cend() const
{
    return const_iterator(faces, vertices, normals, (Index)faces.size());
}

