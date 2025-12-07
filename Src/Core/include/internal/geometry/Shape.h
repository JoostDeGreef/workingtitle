#pragma once

#include "internal/geometry/BoundingObject.h"
#include "internal/geometry/Edges.h"
#include "internal/geometry/Faces.h"
#include "internal/geometry/Normals.h"
#include "internal/geometry/Transformation.h"
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

    // Translate the shape
    void translate(const Vertex & translation);

    // Rotate the shape
    void rotate(const Normal& axis, const Scalar& angle);

    // Calculate the shape surface area
    Scalar calculateSurfaceArea() const;

    // Calculate the shape volume
    Scalar calculateVolume() const;

    // Determine if shapes overlap
    bool detectCollision(const Shape& other) const;
private:
    // The transformation (rotation + translation only) to apply on the vertices
    Transformation transformation;

    // The points (vertices) of each face
    Faces faces;

    // Vertices are stored in the shape:
    // - to save space (same vertex used in multiple faces)
    // - so transformations can be applied on all vertices at once
    Vertices vertices;
    
    // The vertices with the transformation applied
    mutable Vertices transformedVertices;

    // The normals for each face (volatile data)
    mutable Normals normals;

    // The normals with the transformation applied (volatile data)
    mutable Normals transformedNormals;

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
    void invalidateTransformedNormals() const;
    void invalidateSurfaceAreas() const;
    void invalidateTransformedVertices() const;

    // Initialize volatile data if needed
    void requireBounds() const;
    void requireEdges() const;
    void requireNormals() const;
    void requireTransformedNormals() const;
    void requireSurfaceAreas() const;
    void requireTransformedVertices() const;
};

