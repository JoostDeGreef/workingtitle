#pragma once

template<typename VALUE>
class BoundingObject
{
public:
    enum class Type
    {
        None = 0,
        Sphere = 1,
        Box = 2,
        Auto = 3,
    };
    struct Box
    {
        Vertex<VALUE> min;
        Vertex<VALUE> max;
    };
    struct Sphere
    {
        Vertex<VALUE> center;
        VALUE radius;
    };

    BoundingObject();
    BoundingObject(
        const Vertex<VALUE>& min,
        const Vertex<VALUE>& max);
    BoundingObject(
        const Vertex<VALUE>& center,
        const VALUE& radius);
    BoundingObject(const BoundingObject<VALUE>& other);
    BoundingObject(BoundingObject<VALUE>&& other) noexcept;
    BoundingObject(const std::vector<Vertex<VALUE>>& vertices, const Type type = Type::Auto);

    BoundingObject<VALUE>& operator = (const BoundingObject<VALUE>& other);
    BoundingObject<VALUE>& operator = (BoundingObject<VALUE>&& other) noexcept;

    void copy(const BoundingObject<VALUE>& other);
    void swap(BoundingObject<VALUE>& other);

    void create(const std::vector<Vertex<VALUE>>& vertices, const Type type);

    Type getType() const;

    VALUE calculateVolume() const;

    bool overlap(const BoundingObject<VALUE> &other) const;

private:
    void createBox(const std::vector<Vertex<VALUE>>& vertices);
    void createSphere(const std::vector<Vertex<VALUE>>& vertices);

    Type type;
    union
    {
        Box box;
        Sphere sphere;
    };
};

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject()
    : type(Type::None)
{}

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject(const Vertex<VALUE>&min, const Vertex<VALUE>&max)
    : type(Type::Box)
{
    box.min = min;
    box.max = max;
}

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject(const Vertex<VALUE>& center, const VALUE& radius)
    : type(Type::Sphere)
{
    sphere.center = center;
    sphere.radius = radius;
}

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject(const BoundingObject<VALUE>& other)
{
    copy(other);
}

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject(BoundingObject<VALUE>&& other) noexcept
    : type(Type::None)
{
    swap(other);
}

template<typename VALUE>
inline BoundingObject<VALUE>::BoundingObject(const std::vector<Vertex<VALUE>>& vertices, const Type type)
{
    create(vertices, type);
}

template<typename VALUE>
inline BoundingObject<VALUE>& BoundingObject<VALUE>::operator=(const BoundingObject<VALUE>& other)
{
    copy(other);
    return *this;
}

template<typename VALUE>
inline BoundingObject<VALUE>& BoundingObject<VALUE>::operator=(BoundingObject<VALUE>&& other) noexcept
{
    swap(other);
    return *this;
}

template<typename VALUE>
inline void BoundingObject<VALUE>::copy(const BoundingObject<VALUE>& other)
{
    type = other.type;
    switch (type)
    {
    case Type::Auto:
    case Type::None:
        break;
    case Type::Box:
        box = other.box;
        break;
    case Type::Sphere:
        sphere = other.sphere;
        break;
#ifndef NDEBUG
    default:
        throw out_of_range("unhandled type");
#endif
    }
}

template<typename VALUE>
inline void BoundingObject<VALUE>::swap(BoundingObject<VALUE>& other)
{
    switch (type)
    {
    case Type::Auto:
    case Type::None:
        copy(other);
        other.type = Type::None;
        break;
    case Type::Box:
        {
            Box tmp = box;
            copy(other);
            other.type = Type::Box;
            other.box = tmp;
        }
        break;
    case Type::Sphere:
        {
            Sphere tmp = sphere;
            copy(other);
            other.type = Type::Sphere;
            other.sphere = tmp;
        }
        break;
    }
}

template<typename VALUE>
inline void BoundingObject<VALUE>::create(const std::vector<Vertex<VALUE>>& vertices, const Type type)
{
    switch (type)
    {
    case Type::None:
        this->type = Type::None;
        break;
    case Type::Auto:
        {
            auto boBox = BoundingObject<VALUE>(vertices, Type::Box);
            auto boSphere = BoundingObject<VALUE>(vertices, Type::Sphere);
            auto boBoxVolume = boBox.calculateVolume();
            auto boSphereVolume = boSphere.calculateVolume();
            if ( boBoxVolume <= boSphereVolume )
            {
                copy(boBox);
            }
            else
            {
                copy(boSphere);
            }
        }
        break;
    case Type::Box:
        createBox(vertices);
        break;
    case Type::Sphere:
        createSphere(vertices);
        break;
#ifndef NDEBUG
    default:
        throw out_of_range("unhandled type");
#endif
    }
}

template<typename VALUE>
inline typename BoundingObject<VALUE>::Type BoundingObject<VALUE>::getType() const
{
    return type;
}

template<typename VALUE>
inline VALUE BoundingObject<VALUE>::calculateVolume() const
{
    switch (type)
    {
    case Type::Box:
        return (box.max.x-box.min.x) * (box.max.y - box.min.y) * (box.max.z - box.min.z);
    case Type::Sphere:
        return 4 * sphere.radius * sphere.radius * sphere.radius * Constants::Pi / 3;
    case Type::None:
    case Type::Auto:
        return VALUE(-1);
    }
#ifndef NDEBUG
    throw out_of_range("unhandled type");
#endif
}

template<typename VALUE>
inline bool BoundingObject<VALUE>::overlap(const BoundingObject<VALUE>& other) const
{
    if (type == Type::Box)
    {
        if (other.type == Type::Box)
        {
            return
                box.max.x >= other.box.min.x &&
                box.min.x <= other.box.max.x &&
                box.max.y >= other.box.min.y &&
                box.min.y <= other.box.max.y &&
                box.max.z >= other.box.min.z &&
                box.min.z <= other.box.max.z;
        }
        else if (other.type == Type::Sphere)
        {
            return other.overlap(*this);
        }
    }
    else if (type == Type::Sphere)
    {
        if (other.type == Type::Box)
        {
            if( sphere.center.x + sphere.radius >= other.box.min.x &&
                sphere.center.x - sphere.radius <= other.box.max.x &&
                sphere.center.y + sphere.radius >= other.box.min.y &&
                sphere.center.y - sphere.radius <= other.box.max.y &&
                sphere.center.z + sphere.radius >= other.box.min.z &&
                sphere.center.z - sphere.radius <= other.box.max.z)
            {
                Vertex<VALUE> corner = sphere.center;
                for (size_t i = 0; i < 3; ++i)
                {
                    if (sphere.center[i] <= other.box.min[i]) corner[i] = other.box.min[i];
                    else if (sphere.center[i] >= other.box.max[i]) corner[i] = other.box.max[i];
                }
                if(sphere.center.dist(corner) > sphere.radius)
                {
                    return false;
                }
                return true;
            }
            return false;
        }
        else if (other.type == Type::Sphere)
        {
            auto vec = sphere.center - other.sphere.center;
            return (vec.innerProduct(vec) <= Numerics::Sqr(sphere.radius + other.sphere.radius));
        }
    }
    return false;
}

template<typename VALUE>
inline void BoundingObject<VALUE>::createBox(const std::vector<Vertex<VALUE>>& vertices)
{
    type = Type::Box;
    box.min = Vertex<VALUE>(Limits<VALUE>::MaxValue, Limits<VALUE>::MaxValue, Limits<VALUE>::MaxValue);
    box.max = Vertex<VALUE>(Limits<VALUE>::MinValue, Limits<VALUE>::MinValue, Limits<VALUE>::MinValue);
    for (const auto& vertex : vertices)
    {
        box.min.x = std::min(box.min.x, vertex.x);
        box.min.y = std::min(box.min.y, vertex.y);
        box.min.z = std::min(box.min.z, vertex.z);
        box.max.x = std::max(box.max.x, vertex.x);
        box.max.y = std::max(box.max.y, vertex.y);
        box.max.z = std::max(box.max.z, vertex.z);
    }
}

template<typename VALUE>
inline void BoundingObject<VALUE>::createSphere(const std::vector<Vertex<VALUE>>& vertices)
{
    // function to find the vertex from the list furthest from the pivot
    auto findFurthestVertex = [](const Vertex<VALUE> &pivot, const std::vector<Vertex<VALUE>>& vertices)
        {
            VALUE dist2 = 0;
            Vertex<VALUE> p = pivot;
            for (const auto& vertex : vertices)
            {
                auto tmpDist2 = vertex.dist2(pivot); // radius^2
                if (tmpDist2 > dist2)
                {
                    p = vertex;
                    dist2 = tmpDist2;
                }
            }
            return p;
        };
    // function to find the vertex outside the circle and update the circle (center + radius^2)
    auto findOutliers = [](Vertex<VALUE>& center, VALUE radius2, const std::vector<Vertex<VALUE>>& vertices)
        {
            std::vector<Vertex<VALUE>> outliers;
            VALUE dist2Max = radius2;
            Vertex<VALUE> furthest = center;
            for (const auto& vertex : vertices)
            {
                auto dist2 = vertex.dist2(center);
                if (dist2 > radius2)
                {
                    outliers.emplace_back(vertex);
                    if (dist2 > dist2Max)
                    {
                        dist2Max = dist2;
                        furthest = vertex;
                    }
                }
            }
            if (!outliers.empty())
            {
                auto spoke = furthest - center;
                auto spokeLength = Numerics::Sqrt(spoke.innerProduct(spoke));
                auto diff = spokeLength - Numerics::Sqrt(radius2);
                center += spoke * (diff / (2 * spokeLength));
                radius2 = Numerics::Sqr(spokeLength - diff / 2);
            }
            return outliers;
        };
    // determine cloud center
    Vertex<VALUE> center;
    for (const auto& vertex : vertices)
    {
        center += vertex;
    }
    center /= vertices.size();
    // find vertex p1 furthest from center
    auto p1 = findFurthestVertex(center, vertices);
    // find vertex p2 furthest from p1 
    auto p2 = findFurthestVertex(p1, vertices);
    // calculate center and radius squared
    center = (p1 + p2) / 2;
    auto radius2 = (p1 - p2).innerProduct(p1 - p2) / 4; // /4, this is radius^2
    // find vertices outside of initial sphere
    std::vector<Vertex<VALUE>> outliers = findOutliers(center, radius2, vertices);
    // increase bouding sphere untill there are no more outliers
    while (!outliers.empty())
    {
        outliers = findOutliers(center, radius2, outliers);
    }
    // store the results
    type = Type::Sphere;
    sphere.center = center;
    sphere.radius = Numerics::Sqrt(radius2);
}
