#include <fstream>
#include <iostream>
#include <sstream>

#include "internal/geometry/Points.h"

#include "internal/utilities/svg.h"

void SVG::writeToStream(std::ostream& os) const
{
    // header
    os << "<svg xmlns = \"http://www.w3.org/2000/svg\"\n"
          "     xmlns:xlink = \"http://www.w3.org/1999/xlink\"\n"
          "     width = \"" << width << "\" height = \"" << height << "\" viewBox = \"" << viewBox << "\">\n";
    // write axis
       // TODO
    // sort the objects by depth
       // TODO
    // write all objects
    for (const auto& object : renderObjects)
    {
        os << object;
    }
    // footer
    os << "</svg>\n";
}

void SVG::writeToFile(const std::string& filename) const
{
    std::fstream f(filename, std::fstream::out | std::fstream::trunc);
    writeToStream(f);
}

std::string SVG::writeToString() const
{
    std::stringstream s;
    writeToStream(s);
    return s.str();
}

void SVG::addShape(const Shape& shape, const Point& center, const View view)
{
    for (const auto& face : shape.getTransformedFaces())
    {
        Points points;
        points.reserve(face.size());
        switch (view)
        {
            case View::XY:
                for (const auto& vertex : face)
                {
                    // decide to cull based on face.getNormal()
                    // add something like a z-index?
                    points.emplace_back(vertex.x, vertex.y);
                }
                break;
            case View::XZ:
                break;
            case View::YZ:
                break;
            case View::Ortho:
                break;
        }
        renderObjects.emplace_back(renderFormat, points);
    }
}

std::ostream& operator << (std::ostream& os, const SVG& svg)
{
    svg.writeToStream(os);
    return os;
}

std::ostream& operator << (std::ostream& os, const SVG::ViewBox& viewBox)
{
    return os << viewBox.min.x << " " << viewBox.min.y << " " << viewBox.max.x << " " << viewBox.max.y;
}

std::ostream& operator<<(std::ostream& os, const SVG::RenderObject::Path& path)
{
    return os << "PATH\n";
}

std::ostream& operator<<(std::ostream& os, const SVG::RenderObject& object)
{
    std::visit([&os](auto& arg) { os << arg; }, object.data);
    return os;
}

