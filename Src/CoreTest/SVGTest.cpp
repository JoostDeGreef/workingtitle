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
    style.fill = SVG::Color::Predefined::None;
    style.stroke = SVG::Color::Predefined::Black;
    svg.addAxis(      {  50, 50 }, 30, SVG::View::XY);
    svg.addAxis(      {  50,150 }, 30, SVG::View::YZ);
    svg.addAxis(      { 150, 50 }, 30, SVG::View::XZ);
    svg.addAxis(      { 150,150 }, 30, SVG::View::Ortho);
    style.fill = SVG::Color::Predefined::Red;
    style.stroke = SVG::Color::RGB(255, 0, 50);
    svg.addShape(box, {  50, 50 },     SVG::View::XY);
    svg.addShape(box, {  50,150 },     SVG::View::YZ);
    svg.addShape(box, { 150, 50 },     SVG::View::XZ);
    svg.addShape(box, { 150,150 },     SVG::View::Ortho);
    svg.writeToFile("Box.svg");
}
