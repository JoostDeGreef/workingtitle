#include <map>
#include <numeric>
#include <tuple>

#include "internal/geometry/Shape.h"

Shape::Shape()
    : vertices()
    , faces()
    , transformation()
    , normals()
    , edges()
    , surfaceAreas()
    , bounds()
{}

Shape::Shape(const Vertices & vertices, const Faces & faces)
    : vertices(vertices)
    , faces(faces)
    , transformation()
    , normals()
    , edges()
    , surfaceAreas()
    , bounds()
{
    optimize();
}

Shape::Shape(const Shape& other)
    : vertices(other.vertices)
    , faces(other.faces)
    , transformation(other.transformation)
    , normals(other.normals)
    , edges(other.edges)
    , surfaceAreas(other.surfaceAreas)
    , bounds(other.bounds)
{}

Shape::Shape(Shape&& other) noexcept
    : vertices(std::move(other.vertices))
    , faces(std::move(other.faces))
    , transformation(std::move(other.transformation))
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
    if (!transformedVertices.empty())
    {
        for (auto& vertex : transformedVertices)
        {
            vertex *= factor;
        }
    }
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
        invalidateTransformedVertices();
    }
}

void Shape::translate(const Vertex& translation)
{
    transformation *= Transformation(translation);
    invalidateBounds();
    invalidateTransformedNormals();
    invalidateTransformedVertices();
}

void Shape::rotate(const Normal& axis, const Scalar& angle)
{
    transformation *= Transformation(axis, angle);
    invalidateBounds();
    invalidateTransformedNormals();
    invalidateTransformedVertices();
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
//    requireEdges();
//    other.requireEdges();
    requireTransformedVertices();
    other.requireTransformedVertices();
    // check faces using eventline
    struct MaxVertex
    {
        bool operator()(const Vertex & a, const Vertex & b) const 
        { 
            return (a.z > b.z) || (a.z == b.z && a.x > b.x) || (a.z == b.z && a.x == b.x && a.y > b.y);
        }
    };
    struct Event
    {
        Event(const Shape& shape, const Face& face)
        {
            Vertices vertices;
            vertices.reserve(face.size());
            for (const auto& i : face)
            {
                vertices.emplace_back(shape.vertices[i]);
            }
            std::sort(vertices.begin(), vertices.end(), MaxVertex());
            max = vertices.front();
            min = vertices.back();
        }
        Vertex max;
        Vertex min;
    };
    struct MaxEvent
    {
        bool operator()(const Event& a, const Event& b) const
        {
            return MaxVertex()(a.max, b.max);
        }
    };
    std::vector<Event> events;
    events.reserve(faces.size() + other.faces.size());
    for (const auto& face : faces)
    {
        events.emplace_back(*this, face);
    }
    for (const auto& face : other.faces)
    {
        events.emplace_back(other, face);
    }
    std::sort(events.begin(), events.end(), MaxEvent());
    // TODO

    return true;
}

const Vertices& Shape::getVertices() const
{
    return vertices;
}

const Vertices& Shape::getTransformedVertices() const
{
    requireTransformedVertices();
    return transformedVertices;
}

const Normals& Shape::getNormals() const
{
    requireNormals();
    return normals;
}

const Normals& Shape::getTransformedNormals() const
{
    requireTransformedNormals();
    return transformedNormals;
}

const Faces& Shape::getFaces() const
{
    return faces;
}

FaceVisitor Shape::getFace(const size_t& index) const
{
    requireNormals();
    return FaceVisitor(faces.at(index), vertices, normals.at(index));
}

const Face& Shape::getIndexFace(const size_t& index) const
{
    return faces.at(index);
}

FaceVisitor Shape::getTransformedFace(const size_t& index) const
{
    requireTransformedVertices();
    requireTransformedNormals();
    return FaceVisitor(faces.at(index), transformedVertices, transformedNormals.at(index));
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

void Shape::invalidateTransformedNormals() const
{
    transformedNormals.clear();
}

void Shape::invalidateSurfaceAreas() const
{
    surfaceAreas.clear();
}

void Shape::invalidateTransformedVertices() const
{
    transformedVertices.clear();
}

void Shape::requireBounds() const
{
    requireTransformedVertices();
    if (!bounds)
    {
        bounds.create(transformedVertices);
    }
}

void Shape::requireEdges() const
{
    if (edges.empty())
    {
        // [start point, end point] => [face,pre start point,after end point,edgeIdx]
        std::map<std::tuple<Index, Index>, std::tuple<Index,Index,Index,Index>> edgesMap;
        for (Index faceIdx = 0; faceIdx < faces.size(); ++faceIdx)
        {
            const auto& face = faces[faceIdx];
            Index prev = face[face.size() - 2];
            Index start = face[face.size() - 1];
            for (Index end = 0; end < face.size(); ++end)
            {
                Index next = (end + 1) % face.size();
                edgesMap[std::make_tuple(start,end)] = std::make_tuple(faceIdx,prev,next,(Index)edgesMap.size());
                prev = start;
                start = end;
            }
        }
        edges.resize(edgesMap.size());
        for (auto& edgeData : edgesMap)
        {
            auto& edge = edges[std::get<3>(edgeData.second)];
            edge.startVertex = std::get<0>(edgeData.first);
            edge.endVertex = std::get<1>(edgeData.first);
            edge.face = std::get<0>(edgeData.second);
            edge.mirrorEdge = std::get<3>(edgesMap[std::make_tuple(edge.endVertex, edge.startVertex)]);
            edge.prevEdge = std::get<3>(edgesMap[std::make_tuple(std::get<1>(edgeData.second), edge.startVertex)]);
            edge.nextEdge = std::get<3>(edgesMap[std::make_tuple(edge.endVertex, std::get<2>(edgeData.second))]);
        }
    }
}

// Helper for requireNoemal and requireTransformedNormal
Normal faceNormal(const Face& face, const Vertices & vertices)
{
    assert(face.size() > 2); // degenerative face, no normal
    Vertex s0;
    Vertex s1 = vertices.at(face[face.size() - 2]) - vertices.at(face[face.size() - 1]);
    Normal n;
    for (Index i = 0, j = face.size() - 1; i < face.size(); j = i, ++i)
    {
        s0 = s1;
        s1 = vertices.at(face[i]) - vertices.at(face[j]);
        n += s0.crossProduct(s1);
    }
    return n.normalize();
};

void Shape::requireNormals() const
{
    if (normals.empty())
    {
        normals.reserve(faces.size());
        for (const auto& face : faces)
        {
            normals.emplace_back(faceNormal(face, vertices));
        }
    }
}

void Shape::requireTransformedNormals() const
{
    if (transformedNormals.empty())
    {
        requireTransformedVertices();
        transformedNormals.reserve(faces.size());
        for (const auto& face : faces)
        {
            transformedNormals.emplace_back(faceNormal(face, transformedVertices));
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

void Shape::requireTransformedVertices() const
{
    if (transformedVertices.empty())
    {
        transformedVertices = transformation * vertices;
    }
}
