#pragma once

#include "internal/generic/Normals.h"
#include "internal/generic/Vertices.h"

#include "internal/geometry/BoundingObject.h"
#include "internal/geometry/Edges.h"
#include "internal/geometry/Faces.h"
#include "internal/geometry/FaceVisitor.h"
#include "internal/geometry/FacesVisitor.h"
#include "internal/geometry/Transformation.h"

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

    // Get vertices
    const Vertices& getVertices() const;
    const Vertices& getTransformedVertices() const;

    // Get normals
    const Normals& getNormals() const;
    const Normals& getTransformedNormals() const;

    // Get faces
    const Faces& getRawFaces() const;
    const Face& getRawFace(const size_t& index) const;
    FacesVisitor getFaces() const;
    FacesVisitor getTransformedFaces() const;
    FaceVisitor getFace(const size_t& index) const;
    FaceVisitor getTransformedFace(const size_t& index) const;
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

    // Remove unused vertices
    void optimize();

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

inline FacesVisitor Shape::getFaces() const
{
    requireNormals();
    return FacesVisitor(faces,vertices,normals);
}

inline FacesVisitor Shape::getTransformedFaces() const
{
    requireTransformedNormals();
    requireTransformedVertices();
    return FacesVisitor(faces, transformedVertices, transformedNormals);
}


