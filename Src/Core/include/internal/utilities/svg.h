#pragma once

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

    struct RenderFormat
    {
        std::string fillColor;
        std::string lineColor;
    };

    struct RenderObject
    {
        struct Path
        {
            Path(const Points& points)
                : points(points)
            {}

            const Points& points;

            friend std::ostream& operator <<(std::ostream& os, const RenderObject::Path& path);
        };

        RenderObject(std::shared_ptr<RenderFormat> & renderFormat, const Points & points)
            : renderFormat(renderFormat)
            , data(points)
        {}

        std::shared_ptr<RenderFormat> renderFormat;

        friend std::ostream& operator <<(std::ostream& os, const RenderObject& object);
    private:
        std::variant<Path> data;
    };

    SVG(const int width, const int height, const ViewBox& viewBox)
        : width(width)
        , height(height)
        , viewBox(viewBox)
        , renderFormat(new RenderFormat)
        , renderObjects()
    {}

    void writeToStream(std::ostream & os) const;
    void writeToFile(const std::string & filename) const;
    std::string writeToString() const;

    void setFillColor(const std::string& color)
    {
        getUniqueRenderFormat().fillColor = color;
    }
    void setLineColor(const std::string& color)
    {
        getUniqueRenderFormat().lineColor = color;
    }

    void addShape(const Shape& shape, const Point & center, const View view);

    friend std::ostream& operator << (std::ostream& os, const SVG& svg);
private:
    const int width;
    const int height;
    const ViewBox viewBox;

    RenderFormat& getUniqueRenderFormat()
    {
        if (!renderFormat.unique())
        {
            renderFormat.reset(new RenderFormat(*renderFormat));
        }
        return *renderFormat;
    }

    std::shared_ptr<RenderFormat> renderFormat;
    std::vector<RenderObject> renderObjects;
};

std::ostream& operator << (std::ostream& os, const SVG& svg);
std::ostream& operator << (std::ostream& os, const SVG::ViewBox& viewBox);
std::ostream& operator<<(std::ostream& os, const SVG::RenderObject& object);
