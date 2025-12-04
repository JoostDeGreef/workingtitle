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

TEST_F(ShapeTest, Box)
{
    Shape box = ShapeFactory::Box();
    EXPECT_FLOAT_EQ(6, box.calculateSurfaceArea());
    EXPECT_FLOAT_EQ(1, box.calculateVolume());
    box.scale(3);
    EXPECT_FLOAT_EQ(3 * 3 * 3, box.calculateVolume());
    EXPECT_FLOAT_EQ(3 * 3 * 6, box.calculateSurfaceArea());
    box.optimize();
}

TEST_F(ShapeTest, Octahedron)
{
    Shape octahedron = ShapeFactory::Octahedron();
    Scalar A = sqrt(48.0);
    Scalar V = 8.0 / 6.0;
    EXPECT_FLOAT_EQ(A, octahedron.calculateSurfaceArea());
    EXPECT_FLOAT_EQ(V, octahedron.calculateVolume());
    octahedron.scale(3);
    EXPECT_FLOAT_EQ(V * 3 * 3 * 3, octahedron.calculateVolume());
    EXPECT_FLOAT_EQ(A * 3 * 3, octahedron.calculateSurfaceArea());
    octahedron.optimize();
}

TEST_F(ShapeTest, Dodecahedron)
{
    Shape dodecahedron = ShapeFactory::Dodecahedron();
    Scalar ϕ = (1.0 + sqrt(5.0)) / 2.0;
    Scalar a = 2.0 / (ϕ * sqrt(3.0));
    Scalar A = 3.0 * a * a * sqrt(5.0 * (3 + 4 * ϕ));
    EXPECT_FLOAT_EQ(A, dodecahedron.calculateSurfaceArea());
    Scalar V = a * a * a * (4 + 7 * ϕ) / 2.0;
    EXPECT_FLOAT_EQ(V, dodecahedron.calculateVolume());
    dodecahedron.scale(3);
    EXPECT_FLOAT_EQ(V * 3 * 3 * 3, dodecahedron.calculateVolume());
    EXPECT_FLOAT_EQ(A * 3 * 3, dodecahedron.calculateSurfaceArea());
    dodecahedron.optimize();
}

TEST_F(ShapeTest, Extrusion)
{
    Shape extrusion = ShapeFactory::Extrusion();
    EXPECT_FLOAT_EQ(6, extrusion.calculateSurfaceArea());
    EXPECT_FLOAT_EQ(1, extrusion.calculateVolume());
    extrusion.scale(3);
    EXPECT_FLOAT_EQ(3 * 3 * 3, extrusion.calculateVolume());
    EXPECT_FLOAT_EQ(3 * 3 * 6, extrusion.calculateSurfaceArea());
    extrusion.optimize();
}


