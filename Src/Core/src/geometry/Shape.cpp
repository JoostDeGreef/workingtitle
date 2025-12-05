#include <numeric>

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
    invalidateSurfaceAreas();
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
    return std::accumulate(surfaceAreas.begin(), surfaceAreas.end(), 0.0);
}

Scalar Shape::calculateVolume() const
{
    // Make required volatile data available
    requireNormals();
    requireSurfaceAreas();
    // iterate over the faces
    auto faceVolume = [&](const Face& face, const Normal & normal, const Scalar & surface)
    {
        assert(face.size() > 2); // degenerative face, no surface area
        Scalar volume = 0;
        // is it really neccesairy to do this for all vertices?
        for (const auto i : face)
        {
            volume += surface * vertices[i].innerProduct(normal);
        }
        return volume / (3 * face.size());
    };
    Scalar res = 0;
    for (size_t i = 0; i < faces.size(); ++i)
    {
        res += faceVolume(faces[i],normals[i],surfaceAreas[i]);
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
    // Make required volatile data available
    requireEdges();
    other.requireEdges();
    // TODO: do expensive check

    return true;
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
    auto faceNormal = [&](const Face & face)
    {
        assert(face.size() > 2); // degenerative face, no normal
        Vertex s0;
        Vertex s1 = vertices.at(face[face.size() - 2]) - vertices.at(face[face.size() - 1]);
        Normal n;
        for (Index i = 0, j = face.size() - 1; i < face.size(); j = i, ++i)
        {
            s0 = s1;
            s1 = vertices.at(face[i]) - vertices.at(face[j]);
            n += s1.crossProduct(s0);
        }
        return n.normalize();
    };
    if (normals.empty())
    {
        normals.reserve(faces.size());
        for (const auto& face : faces)
        {
            normals.emplace_back(faceNormal(face));
        }
    }
}

void Shape::requireSurfaceAreas() const
{
    auto faceSurfaceArea = [&](const Face& face)
    {
        assert(face.size() > 2); // degenerative face, no surface area
        Vertex area(0, 0, 0);
        Vertex s0;
        const Vertex& v0 = vertices.at(face[0]);
        Vertex s1 = vertices.at(face[1]) - v0;
        for (Index i = 2; i < face.size(); ++i)
        {
            s0 = s1;
            s1 = vertices.at(face[i]) - v0;
            area += s0.crossProduct(s1);
        }
        return area.length() / 2;
    };
    if (surfaceAreas.empty())
    {
        surfaceAreas.reserve(faces.size());
        for (const auto& face : faces)
        {
            surfaceAreas.emplace_back(faceSurfaceArea(face));
        }
    }
}
