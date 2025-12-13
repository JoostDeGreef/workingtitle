#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <variant>

#include "internal/geometry/Points.h"

#include "internal/utilities/svg.h"

std::map<SVG::Color::Predefined, std::string> SVG::Color::predefinedColors =
{
    {SVG::Color::Predefined::None, "none"},
    {SVG::Color::Predefined::Black, "black"},
    {SVG::Color::Predefined::Red, "red"},
};

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
                    points.emplace_back(center.x + vertex.x, center.y + vertex.y);
                }
                break;
            case View::XZ:
                for (const auto& vertex : face)
                {
                    points.emplace_back(center.x + vertex.x, center.y + vertex.z);
                }
                break;
            case View::YZ:
                for (const auto& vertex : face)
                {
                    points.emplace_back(center.x + vertex.y, center.y + vertex.z);
                }
                break;
            case View::Ortho:
                break;
        }
        renderObjects.emplace_back(style, points);
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

std::ostream& operator<<(std::ostream& os, const SVG::Color& color)
{
    struct ColorWriter
    {
        std::ostream& os;
        std::ostream& operator () (const SVG::Color::RGB& rgb) 
        { 
            return os << "#" << std::hex << std::right << std::setfill('0') << std::setw(2) << rgb.r << std::setw(2) << rgb.g << std::setw(2) << rgb.b << std::dec;
        }
        std::ostream& operator () (const SVG::Color::Predefined& p) 
        { 
            return os << SVG::Color::predefinedColors[p];
        }
    };
    return std::visit(ColorWriter{os}, color.data);
}

std::ostream& operator<<(std::ostream& os, const SVG::Style& style)
{
    //<style>
    //    #a
    //{
    //  fill: gold;
    //  stroke: maroon;
    //  stroke - width: 2px;
    //}
    //    < / style>

    return 
        os << "style=\"fill:" << style.fill << ";"
           << "fill-opacity:" << style.fillOpacity << ";"
           << "fill-rule:" << (style.fillRule == SVG::FillRule::nonzero ? "nonzero" : "evenodd") << ";"
           << "stroke:" << style.stroke << ";"
           << "stroke-width:" << style.strokeWidth << ";"
           << "stroke-opacity:" << style.strokeOpacity << "\"";
}

std::ostream& operator<<(std::ostream& os, const SVG::RenderObject::Path& path)
{
    if (!path.points.empty())
    {
        os << "<path d=\"M" << std::setprecision(5);
        for (const auto& p : path.points)
        {
            os << p.x << "," << p.y << " ";
        }
        const auto& p = path.points.front();
        return os << p.x << "," << p.y << "\"";
    }
    else
    {
        return os << "<path";
    }
}

std::ostream& operator<<(std::ostream& os, const SVG::RenderObject& object)
{
    std::visit([&os](auto& arg) { os << arg; }, object.data);
    os << " " << *object.style << "/>\n";
    return os;
}

