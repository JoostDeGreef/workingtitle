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
    svg.addAxis(      {  50, 50, 0 }, 30);
    svg.addAxis(      {  50,150, 0 }, 30);
    svg.addAxis(      { 150, 50, 0 }, 30);
    svg.addAxis(      { 150,150, 0 }, 30);
    style.fill = SVG::Color::Predefined::Red;
    style.stroke = SVG::Color::RGB(255, 0, 50);
    svg.addShape(box, {  50, 50, 0 });
    svg.addShape(box, {  50,150, 0 });
    svg.addShape(box, { 150, 50, 0 });
    svg.addShape(box, { 150,150, 0 });
    svg.writeToFile("Box.svg");
}
