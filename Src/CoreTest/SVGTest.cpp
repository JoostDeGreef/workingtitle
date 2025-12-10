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
    Shape box = ShapeFactory::Box();
    SVG svg(640, 480, {0,0,320,240});
    svg.writeToFile("Box.svg");
}
