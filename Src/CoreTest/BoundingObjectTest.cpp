#include "gtest/gtest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class BoundingObjectTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(BoundingObjectTest, ConstructorNone)
{
    BoundingObject<double> b1;
    BoundingObject<double> b2(b1);
    EXPECT_EQ(BoundingObject<double>::Type::None, b2.getType());
    BoundingObject<double> b3(std::move(b1));
    EXPECT_EQ(BoundingObject<double>::Type::None, b3.getType());
}

TEST_F(BoundingObjectTest, ConstructorBox)
{
    BoundingObject<double> b1({ 1,2,3 }, { 2,3,4 });
    BoundingObject<double> b2(b1);
    EXPECT_EQ(BoundingObject<double>::Type::Box, b2.getType());
    BoundingObject<double> b3(std::move(b1));
    EXPECT_EQ(BoundingObject<double>::Type::Box, b3.getType());
}

TEST_F(BoundingObjectTest, ConstructorSphere)
{
    BoundingObject<double> b1({ 1,2,3 }, 2);
    BoundingObject<double> b2(b1);
    EXPECT_EQ(BoundingObject<double>::Type::Sphere, b2.getType());
    BoundingObject<double> b3(std::move(b1));
    EXPECT_EQ(BoundingObject<double>::Type::Sphere, b3.getType());
}

TEST_F(BoundingObjectTest, ConstructorVertices)
{
    std::vector<Vertex<double>> v1({ {0,0,0},{1,1,1} });
    BoundingObject<double> b1(v1);
    EXPECT_EQ(BoundingObject<double>::Type::Box, b1.getType());
    std::vector<Vertex<double>> v2({ {0,0,0},{2,0,0},{1,1,0},{1,-1,0},{1,0,1},{1,0,-1} });
    BoundingObject<double> b2(v2);
    EXPECT_EQ(BoundingObject<double>::Type::Sphere, b2.getType());
}
