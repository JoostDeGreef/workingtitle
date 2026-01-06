#pragma once

#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "internal/generic/Point.h"
#include "internal/generic/Points.h"
#include "internal/generic/Scalar.h"
#include "internal/generic/Vertex.h"

#include "internal/geometry/Shape.h"
#include "internal/geometry/Transformation.h"

class SVG
{
public:
    struct View
    {
        View(const Vertex & center = Vertex(0,0,0),
             const Vertex & eye = Vertex(1,0,0),
             const Vertex & up = Vertex(0,0,1))
            : center(center)
            , eye(eye)
            , up(up)
        {
            initTransformation();
        }
        View(const View & other)
            : center(other.center)
            , eye(other.eye)
            , up(other.up)
            , transformation(other.transformation)
        {}

        Vertex center;
        Vertex eye;
        Vertex up;

        const Transformation& getTransformation() const
        {
            return transformation;
        }
    protected:
        Transformation transformation;

        void initTransformation();
    };

    struct ViewBox
    {
        ViewBox(const Scalar min_x, const Scalar min_y, const Scalar max_x, const Scalar max_y)
            : min(min_x,min_y)
            , max(max_x,max_y)
        {}

        Point min;
        Point max;

        friend std::ostream& operator <<(std::ostream& os, const ViewBox& viewBox);
    };

    struct Color
    {
        enum class Predefined
        {
            None,
            ContextStroke,
            Black,
            Red,
        };
        struct RGB
        {
            RGB(unsigned char r, unsigned char g, unsigned char b)
                : r(r)
                , g(g)
                , b(b)
            {}
            int r, g, b;

            bool operator == (const RGB& other) const;
            bool operator != (const RGB& other) const;
        };
        Color()
            : data(Predefined::Black)
        {}
        Color(const Predefined p)
            : data(p)
        {}
        Color(const RGB rgb)
            : data(rgb)
        {}
        Color& operator = (const Predefined p)
        {
            data = p;
            return *this;
        }
        Color& operator = (const RGB rgb)
        {
            data = rgb;
            return *this;
        }

        bool operator == (const Color& other) const;
        bool operator != (const Color& other) const;

        friend std::ostream& operator <<(std::ostream& os, const Color& color);
    private:
        std::variant<Predefined,RGB> data;
        static std::map<Predefined, std::string> predefinedColors;
    };

    enum class FillRule
    {
        nonzero,
        evenodd
    };

    enum class StrokeLineJoin
    {
        arcs,
        bevel,
        miter,
        miterclip,
        round
    };

    struct Style
    {
        Style()
            : fill(Color::Predefined::None)
            , stroke(Color::Predefined::Black)
        {}

        Color fill; 
        double fillOpacity = 1; 
        FillRule fillRule = FillRule::nonzero;
        StrokeLineJoin strokeLineJoin = StrokeLineJoin::round;
        Color stroke; 
        double strokeWidth = 1;
        double strokeOpacity = 1;

        std::string id;
        std::string custom;

        bool operator == (const Style & other) const;
        bool operator != (const Style& other) const;

        friend std::ostream& operator <<(std::ostream& os, const Style &style);
    };

    struct Axis
    {
        Axis(const std::string & classId, const Point& tail, const Point& head, const Scalar& z)
            : classId(classId)
            , tail(tail)
            , head(head)
            , z(z)
        {}

        std::string classId;
        Point tail;
        Point head;
        Scalar z;

        friend std::ostream& operator <<(std::ostream& os, const Axis & axis);
    };

    struct Path
    {
        Path(const std::string& classId, const Points& points, const Scalar & z)
            : classId(classId)
            , points(points)
            , z(z)
        {}

        std::string classId;
        Points points;
        Scalar z;

        friend std::ostream& operator <<(std::ostream& os, const Path & path);
    };

    struct Text
    {
        Text(const std::string& classId, const std::string & text, const Point& point, const Scalar& z)
            : classId(classId)
            , text(text)
            , point(point)
            , z(z)
        {
        }

        std::string classId;
        std::string text;
        Point point;
        Scalar z;

        friend std::ostream& operator <<(std::ostream& os, const Text& text);
    };

    SVG(const int width, const int height, const ViewBox& viewBox, const View & view = View());
    
    void setView(const View& view);

    void writeToStream(std::ostream & os);
    void writeToFile(const std::string & filename);
    std::string writeToString();

    Style& getStyle()
    {
        return style;
    }
    void setStyleId(Style &style);

    void addAxis(const Vertex & center, const double length);
    void addShape(const Shape& shape, const Vertex& center);
    void addText(const std::string &text, const Vertex& start);

    friend std::ostream& operator << (std::ostream& os, const SVG& svg);
private:
    const int width;
    const int height;
    const ViewBox viewBox;
    View view;

    Style style;
    std::vector<Style> styles;
    std::vector<std::variant<Axis, Path, Text>> objects;

    Vertex project(const Vertex & v) const;
};

std::ostream& operator << (std::ostream& os, const SVG& svg);
std::ostream& operator << (std::ostream& os, const SVG::Axis& axis);
std::ostream& operator << (std::ostream& os, const SVG::Color& color);
std::ostream& operator << (std::ostream& os, const SVG::Path& path);
std::ostream& operator << (std::ostream& os, const SVG::StrokeLineJoin& strokeLineJoin);
std::ostream& operator << (std::ostream& os, const SVG::Style& style);
std::ostream& operator << (std::ostream& os, const SVG::Text& text);
std::ostream& operator << (std::ostream& os, const SVG::ViewBox& viewBox);
