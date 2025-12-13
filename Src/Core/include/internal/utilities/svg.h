#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <variant>

#include "internal/geometry/Point.h"
#include "internal/geometry/Points.h"

#include "internal/geometry/Shape.h"

class SVG
{
public:
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

    enum class View
    {
        XY,
        XZ,
        YZ,
        Ortho
    };

    struct Color
    {
        enum class Predefined
        {
            None,
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

    struct Style
    {
        Style()
            : fill(Color::Predefined::None)
            , stroke(Color::Predefined::Black)
        {}

        Color fill; 
        double fillOpacity = 1; 
        FillRule fillRule = FillRule::nonzero;
        Color stroke; 
        double strokeWidth = 1;
        double strokeOpacity = 1;

        friend std::ostream& operator <<(std::ostream& os, const Style &style);
    };

    struct RenderObject
    {
        struct Path
        {
            Path(const Points& points)
                : points(points)
            {}

            const Points points;

            friend std::ostream& operator <<(std::ostream& os, const RenderObject::Path& path);
        };

        RenderObject(std::shared_ptr<Style> & style, const Points & points)
            : style(style)
            , data(points)
        {}

        friend std::ostream& operator <<(std::ostream& os, const RenderObject& object);
    private:
        std::shared_ptr<Style> style;
        std::variant<Path> data;
    };

    SVG(const int width, const int height, const ViewBox& viewBox)
        : width(width)
        , height(height)
        , viewBox(viewBox)
        , style(new Style)
        , renderObjects()
    {}

    void writeToStream(std::ostream & os) const;
    void writeToFile(const std::string & filename) const;
    std::string writeToString() const;

    Style& getStyle()
    {
        return getUniqueRenderFormat();
    }

    void addShape(const Shape& shape, const Point & center, const View view);

    friend std::ostream& operator << (std::ostream& os, const SVG& svg);
private:
    const int width;
    const int height;
    const ViewBox viewBox;

    Style& getUniqueRenderFormat()
    {
        if (!style.unique())
        {
            style.reset(new Style(*style));
        }
        return *style;
    }

    std::shared_ptr<Style> style;
    std::vector<RenderObject> renderObjects;
};

std::ostream& operator << (std::ostream& os, const SVG& svg);
std::ostream& operator << (std::ostream& os, const SVG::Color& color);
std::ostream& operator << (std::ostream& os, const SVG::RenderObject& object);
std::ostream& operator << (std::ostream& os, const SVG::RenderObject::Path& path);
std::ostream& operator << (std::ostream& os, const SVG::Style& style);
std::ostream& operator << (std::ostream& os, const SVG::ViewBox& viewBox);
