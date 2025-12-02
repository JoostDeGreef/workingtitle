#include <numeric>

#include "internal/geometry/FaceVisitor.h"
#include "internal/geometry/Shape.h"

Shape::Shape()
    : vertices()
    , faces()
    , normals()
    , edges()
    , surfaceAreas()
    , bounds()
{}

Shape::Shape(const Vertices & vertices, const Faces & faces)
    : vertices(vertices)
    , faces(faces)
    , normals()
    , edges()
    , surfaceAreas()
    , bounds()
{
}

Shape::Shape(const Shape& other)
    : vertices(other.vertices)
    , faces(other.faces)
    , normals(other.normals)
    , edges(other.edges)
    , surfaceAreas(other.surfaceAreas)
    , bounds(other.bounds)
{}

Shape::Shape(Shape&& other) noexcept
    : vertices(std::move(other.vertices))
    , faces(std::move(other.faces))
    , normals(std::move(other.normals))
    , edges(std::move(other.edges))
    , surfaceAreas(std::move(other.surfaceAreas))
    , bounds(std::move(other.bounds))
{}

void Shape::scale(const double& factor)
{
    for (auto& vertex : vertices)
    {
        vertex *= factor;
    }
    // update affected volatile data if present
    if(bounds) bounds.scale(factor);
}

void Shape::optimize()
{
    bool optimizationNeeded = false;
    std::vector<size_t> counts(vertices.size(), 0);
    for (const auto& face : faces)
    {
        for (const auto& point : face)
        {
            optimizationNeeded |= (++counts[point]) > 1;
        }
    }
    if (optimizationNeeded)
    {
        size_t j = 0;
        std::vector<Index> mapping(vertices.size(), 0);
        for (size_t i = 0; i < counts.size(); ++i)
        {
            if (counts[i] != 0)
            {
                vertices[i].swap(vertices[j]);
                mapping[i] = (Index)j++;
            }
        }
        vertices.resize(j);
        for (auto& face : faces)
        {
            for (auto& point : face)
            {
                point = mapping[point];
            }
        }
        // update affected volatile data
        invalidateBounds();
        invalidateSurfaceAreas();
    }
}

Scalar Shape::calculateSurfaceArea() const
{
    // Make required volatile data available
    requireSurfaceAreas();
    return std::accumulate(surfaceAreas.begin(), surfaceAreas.end(), 0);
}

Scalar Shape::calculateVolume() const
{
    // Make required volatile data available
    requireNormals();
    requireSurfaceAreas();
    // iterate over the faces
    Scalar res = 0;
    for (Index i=0;i<faces.size();++i)
    {
        const auto& normal = normals[i];
        const auto& face = faces[i];
        res += (surfaceAreas[i] / 9) * 
             (vertices[face[0]].innerProduct(normal)
            + vertices[face[1]].innerProduct(normal)
            + vertices[face[2]].innerProduct(normal));
    }
    return res;
}

bool Shape::detectCollision(const Shape& other) const
{
    // Make required volatile data available
    requireBounds();
    other.requireBounds();
    if (!bounds.overlap(other.bounds))
    {
        return false;
    }
    else
    {
        // TODO: do expensive check

        return true;
    }    
}

void Shape::invalidateBounds() const
{
    bounds.invalidate();
}

void Shape::invalidateEdges() const
{
    edges.clear();
}

void Shape::invalidateNormals() const
{
    normals.clear();
}

void Shape::invalidateSurfaceAreas() const
{
    surfaceAreas.clear();
}

void Shape::requireBounds() const
{
    if (!bounds)
    {
        bounds.create(vertices);
    }
}

void Shape::requireEdges() const
{
    if (edges.empty())
    {
        // TODO
    }
}

void Shape::requireNormals() const
{
    if (normals.empty())
    {
        normals.reserve(faces.size());
        for (const auto& face : faces)
        {
            normals.emplace_back(FaceVisitor(face,vertices).calculateNormal());
        }
    }
}

void Shape::requireSurfaceAreas() const
{
    if (surfaceAreas.empty())
    {
        surfaceAreas.reserve(faces.size());
        for (const auto& face : faces)
        {
            surfaceAreas.emplace_back(FaceVisitor(face, vertices).calculateSurface());
        }
    }
}
