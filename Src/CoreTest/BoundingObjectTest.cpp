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

TEST_F(BoundingObjectTest, OverlapSphereSphere)
{
    BoundingObject<double> b1({  0,0,0 }, 1);
    BoundingObject<double> b2({ -4,0,0 }, 1);
    BoundingObject<double> b3({ -1,0,0 }, 1);
    BoundingObject<double> b4({  0,0,0 }, 1);
    BoundingObject<double> b5({  1,0,0 }, 1);
    BoundingObject<double> b6({  4,0,0 }, 1);
    EXPECT_FALSE(b1.overlap(b2));
    EXPECT_TRUE(b1.overlap(b3));
    EXPECT_TRUE(b1.overlap(b4));
    EXPECT_TRUE(b1.overlap(b5));
    EXPECT_FALSE(b1.overlap(b6));
}

TEST_F(BoundingObjectTest, OverlapBoxBox)
{
    BoundingObject<double> b1({ -1,-1,-1 }, { 1,1,1});
    BoundingObject<double> b2({ -4,-1,-1 }, {-2,1,1});
    BoundingObject<double> b3({ -2,-1,-1 }, { 0,1,1});
    BoundingObject<double> b4({ -1,-1,-1 }, { 1,1,1});
    BoundingObject<double> b5({  0,-1,-1 }, { 2,1,1});
    BoundingObject<double> b6({  2,-1,-1 }, { 4,1,1});
    EXPECT_FALSE(b1.overlap(b2));
    EXPECT_TRUE(b1.overlap(b3));
    EXPECT_TRUE(b1.overlap(b4));
    EXPECT_TRUE(b1.overlap(b5));
    EXPECT_FALSE(b1.overlap(b6));
}

TEST_F(BoundingObjectTest, OverlapBoxSphere)
{
    BoundingObject<double> b1({  -.1, 0, 0 }, { 2.1,2,2 });
    BoundingObject<double> b2({ -4,-1,-1 }, 2);
    BoundingObject<double> b3({ -2,-1,-1 }, 2);
    BoundingObject<double> b4({ -1,-1,-1 }, 2);
    BoundingObject<double> b5({ 0,-1,-1 }, 2);
    BoundingObject<double> b6({ 3,-1,-1 }, 2);
    BoundingObject<double> b7({ 4,-1,-1 }, 2);
    BoundingObject<double> b8({ 5,-1,-1 }, 2);
    EXPECT_FALSE(b1.overlap(b2));
    EXPECT_FALSE(b1.overlap(b3));
    EXPECT_TRUE(b1.overlap(b4));
    EXPECT_TRUE(b1.overlap(b5));
    EXPECT_TRUE(b1.overlap(b6));
    EXPECT_FALSE(b1.overlap(b7));
    EXPECT_FALSE(b1.overlap(b8));
}

TEST_F(BoundingObjectTest, OverlapSphereBox)
{
    BoundingObject<double> b1({ 0, 0, 1.5 }, 1.1);
    BoundingObject<double> b2({ -4,-1,-1 }, { -2,1,1 });
    BoundingObject<double> b3({ -3,-1,-1 }, { -1,1,1 });
    BoundingObject<double> b4({ -2,-1,-1 }, { 0,1,1 });
    BoundingObject<double> b5({ -1,-1,-1 }, { 1,1,1 });
    BoundingObject<double> b6({  0,-1,-1 }, { 2,1,1 });
    BoundingObject<double> b7({  1,-1,-1 }, { 3,1,1 });
    BoundingObject<double> b8({  2,-1,-1 }, { 4,1,1 });
    EXPECT_FALSE(b1.overlap(b2));
    EXPECT_FALSE(b1.overlap(b3));
    EXPECT_TRUE(b1.overlap(b4));
    EXPECT_TRUE(b1.overlap(b5));
    EXPECT_TRUE(b1.overlap(b6));
    EXPECT_FALSE(b1.overlap(b7));
    EXPECT_FALSE(b1.overlap(b8));
}
