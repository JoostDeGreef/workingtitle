#include "gtest/gtest.h"
#include "Core.h"

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
};

TEST_F(SVGTest, Box)
{
    SVG svg(500, 500, { 0,0,200,200 });
    Shape box = ShapeFactory::Box({-1,-1,-1},{1,1,1});
    box.scale(20);
    auto & style = svg.getStyle();
    style.fillOpacity = 0.5;
    style.fill = SVG::Color::Predefined::Red;
    style.stroke = SVG::Color::RGB(255, 0, 50);
    svg.addShape(box, {  50, 50 }, SVG::View::XY);
    svg.addShape(box, { 150, 50 }, SVG::View::Ortho);
    svg.addShape(box, {  50,150 }, SVG::View::YZ);
    svg.addShape(box, { 150,150 }, SVG::View::XZ);
    svg.writeToFile("Box.svg");
}
