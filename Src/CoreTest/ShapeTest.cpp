#include "gtest/gtest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class ShapeTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ShapeTest, CalculateSurfaceArea)
{
    Shape s = ShapeFactory::Cube();
    EXPECT_EQ(6, s.calculateSurfaceArea());
}

TEST_F(ShapeTest, CalculateVolume)
{
    Shape s = ShapeFactory::Cube();
    EXPECT_EQ(1, s.calculateVolume());
}

TEST_F(ShapeTest, Scale)
{
    Shape s = ShapeFactory::Cube();
    s.scale(3);
    EXPECT_EQ(3*3*3, s.calculateVolume());
    EXPECT_EQ(3*3*6, s.calculateSurfaceArea());
}

TEST_F(ShapeTest, Optimize)
{
    Shape s = ShapeFactory::Cube();
    s.optimize();
}
