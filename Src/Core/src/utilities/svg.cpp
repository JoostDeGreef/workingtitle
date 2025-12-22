#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>

#include "internal/generic/Normal.h"
#include "internal/generic/Points.h"

#include "internal/utilities/svg.h"

std::map<SVG::Color::Predefined, std::string> SVG::Color::predefinedColors =
{
    {SVG::Color::Predefined::None,           "none"},
    {SVG::Color::Predefined::ContextStroke,  "context-stroke"},
    {SVG::Color::Predefined::Black,          "black"},
    {SVG::Color::Predefined::Red,            "red"},
};

SVG::SVG(const int width, const int height, const ViewBox& viewBox, const View& view)
    : width(width)
    , height(height)
    , viewBox(viewBox)
    , view(view)
    , style()
    , styles()
    , renderObjects()
{
    Style arrowTip;
    arrowTip.fill = SVG::Color::Predefined::ContextStroke;
    arrowTip.fillRule = SVG::FillRule::evenodd;
    arrowTip.fillOpacity = 1;
    arrowTip.stroke = SVG::Color::Predefined::ContextStroke;
    arrowTip.strokeWidth = 1;
    arrowTip.strokeOpacity = 1;
    arrowTip.id = "S0";
    styles.emplace_back(arrowTip);
}

void SVG::setView(const View& view)
{
    this->view = view;
}

void SVG::writeToStream(std::ostream& os) const
{
    // header
    os << "<svg xmlns = \"http://www.w3.org/2000/svg\"\n"
          "     xmlns:xlink = \"http://www.w3.org/1999/xlink\"\n"
          "     width = \"" << width << "\" height = \"" << height << "\" viewBox = \"" << viewBox << "\">\n";
    // write styles
    os << "<style>\n";
    for (const auto& style : styles)
    {
        os << style;
    }
    os << "</style>\n";
    // definitions for axis
    os  << "<defs>\n"
        << "  <marker id = \"arrowTip\"\n"
        << "          markerHeight = \"6.155\"\n"
        << "          markerWidth = \"5.324\"\n"
        << "          orient = \"auto-start-reverse\"\n"
        << "          preserveAspectRatio = \"xMidYMid\"\n"
        << "          refX = \"0\"\n"
        << "          refY = \"0\"\n"
        << "          style = \"overflow:visible\"\n"
        << "          viewBox = \"0 0 5.324 6.155\">\n"
        << "    <path d = \"m5.77 0-8.65 5V-5Z\"\n"
        << "          class = \"S0\"\n"
        << "          transform = \"scale(.5)\" />\n"
        << "  </marker>\n"
        << "</defs>\n";
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

void SVG::addAxis(const Vertex& center, const double length)
{
    auto axisStyle = style;
    axisStyle.custom = "marker-end: url(#arrowTip)";
    if (styles.empty() || styles.back() != axisStyle)
    {
        axisStyle.id = "S" + std::to_string(styles.size());
        styles.emplace_back(axisStyle);
    }
    Vertex axis[6] =
    {
        // X
        {center.x - length / 5, center.y, center.z},
        {center.x + length,     center.y, center.z},
        // Y
        {center.x, center.y - length / 5, center.z},
        {center.x, center.y + length,     center.z},
        // Z
        {center.x, center.y, center.z - length / 5},
        {center.x, center.y, center.z + length    },
    };
    for (size_t i = 0; i < 3; ++i)
    {
        Point tail = project(axis[i * 2]);
        Point head = project(axis[i * 2 + 1]);
        renderObjects.emplace_back(styles.back().id, SVG::RenderObject::Axis(tail, head));
    }
}

void SVG::addShape(const Shape& shape, const Vertex& center)
{
    if (styles.empty() || styles.back() != style)
    {
        style.id = "S" + std::to_string(styles.size());
        styles.emplace_back(style);
    }
    for (const auto& face : shape.getTransformedFaces())
    {
        Points points;
        points.reserve(face.size());
        for (const auto& vertex : face)
        {
            auto x = vertex.y - vertex.x*0.3;
            auto y = vertex.z - vertex.x*0.5;
            points.emplace_back(center.x + x, center.y - y);
        }
        renderObjects.emplace_back(styles.back().id, SVG::RenderObject::Path(points));
    }
}

Point SVG::project(const Vertex& v) const
{
    // TODO
    return Point();
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
    return os
        << "." << style.id << "\n"
        << "{\n"
        << "  fill: " << style.fill << "; \n"
        << "  fill-opacity: " << style.fillOpacity << ";\n"
        << "  fill-rule: " << (style.fillRule == SVG::FillRule::nonzero ? "nonzero" : "evenodd") << ";\n"
        << "  stroke: " << style.stroke << ";\n"
        << "  stroke-width: " << style.strokeWidth << ";\n"
        << "  stroke-opacity: " << style.strokeOpacity << ";\n"
        << (style.custom.empty() ? "}\n" : ("  " + style.custom + ";\n}\n"));
}

std::ostream& operator<<(std::ostream& os, const SVG::RenderObject::Axis& axis)
{
    os << "<path d=\"M" << std::setprecision(5);
    os << axis.tail.x << "," << axis.tail.y << " ";
    return os << axis.head.x << "," << axis.head.y << "\"";
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
    os << " class=\"" << object.objectClass << "\" />\n";
    return os;
}

bool SVG::Style::operator==(const Style& other) const
{
    // ignore id here
    return fill == other.fill
        && fillOpacity == other.fillOpacity
        && fillRule == other.fillRule
        && stroke == other.stroke
        && strokeWidth == other.strokeWidth
        && strokeOpacity == other.strokeOpacity
        && custom == other.custom;
}

bool SVG::Style::operator!=(const Style& other) const
{
    return !(*this == other);
}

bool SVG::Color::operator==(const Color& other) const
{
    return data == other.data;
}

bool SVG::Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

bool SVG::Color::RGB::operator==(const RGB& other) const
{
    return r == other.r && g == other.g && b == other.b;
}

bool SVG::Color::RGB::operator!=(const RGB& other) const
{
    return !(*this == other);
}

void SVG::View::initTransformation()
{
    auto axis = Normal(center - eye);
    Scalar angle;

    Transformation t(-center);
    Transformation r(axis, angle);

    transformation = r*t;
}
