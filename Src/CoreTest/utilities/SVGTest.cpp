#include "Core.h"
#include "GoogleTest.h"

using namespace std;
using namespace testing;

class SVGTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    void addShape(SVG & svg, Shape & shape, const Vertex center, const Vertex dir, const Vertex up)
    {
        auto& style = svg.getStyle();
        style.fillOpacity = 1;
        style.fill = SVG::Color::Predefined::None;
        style.stroke = SVG::Color::Predefined::Black;
        svg.setView(SVG::View(center, center - dir, up));
        svg.addAxis({ 0, 0, 0 }, 30);
        style.fillOpacity = 0.5;
        style.fill = SVG::Color::Predefined::Red;
        style.stroke = SVG::Color::RGB(205, 0, 0);
        svg.addShape(shape, center);
    };
};

TEST_F(SVGTest, Box)
{
    SVG svg(500, 500, { 0,0,200,200 });
    Shape box = ShapeFactory::Box({-1,-1,-1},{1,1,1});
    box.scale(20);

    addShape(svg, box, {  50,  50, 0 }, { 1,0,0 }, { 0,-1,0 });
    addShape(svg, box, { 150,  50, 0 }, { 0,1,0 }, { 0,0,-1 });
    addShape(svg, box, {  50, 150, 0 }, { 0,0,1 }, { -1,0,0 });
    addShape(svg, box, { 150, 150, 0 }, { 2,1,1 }, { 0,-1,0 });

    svg.writeToFile("Box.svg");
}

TEST_F(SVGTest, Dodecahedron)
{
    SVG svg(500, 500, { 0,0,200,200 });
    Shape dodecahedron = ShapeFactory::Dodecahedron();
    dodecahedron.scale(40);

    addShape(svg, dodecahedron, { 50,  50, 0 }, { 1,0,0 }, { 0,-1,0 });
    addShape(svg, dodecahedron, { 150,  50, 0 }, { 0,1,0 }, { 0,0,-1 });
    addShape(svg, dodecahedron, { 50, 150, 0 }, { 0,0,1 }, { -1,0,0 });
    addShape(svg, dodecahedron, { 150, 150, 0 }, { 2,1,1 }, { 0,-1,0 });

    svg.writeToFile("Dodecahedron.svg");
}
