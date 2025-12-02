#pragma once

#include "internal/geometry/BoundingObject.h"
#include "internal/geometry/Edges.h"
#include "internal/geometry/Faces.h"
#include "internal/geometry/Normals.h"
#include "internal/geometry/Vertices.h"

class ShapeFactory;

class Shape
{
public:
    friend class ShapeFactory;

    Shape();
    Shape(const Vertices & vertices, const Faces & faces);
    Shape(const Shape & other);
    Shape(Shape && other) noexcept;

    // Scale (multiply) all vertices with this factor
    void scale(const double& factor);

    // Remove unused vertices
    void optimize();

    // Calculate the shape surface area
    Scalar calculateSurfaceArea() const;

    // Calculate the shape volume
    Scalar calculateVolume() const;

    // Determine if shapes overlap
    bool detectCollision(const Shape& other) const;
private:
    // Vertices are stored in the shape:
    // - to save space (same vertex used in multiple faces)
    // - so transformations can be applied on all vertices at once
    Vertices vertices;
    
    // The points of each face
    Faces faces;

    // The normals for each face (volatile data)
    mutable Normals normals;

    // The surface area for each face (volatile data)
    mutable std::vector<Scalar> surfaceAreas;

    // The edges for each face (volatile data)
    mutable Edges edges;

    // Any class inheriting from IBoundingBox (volatile data)  
    mutable BoundingObject bounds;

    // Invalidate volatile data
    void invalidateBounds() const;
    void invalidateEdges() const;
    void invalidateNormals() const;
    void invalidateSurfaceAreas() const;

    // Initialize volatile data if needed
    void requireBounds() const;
    void requireEdges() const;
    void requireNormals() const;
    void requireSurfaceAreas() const;
};

