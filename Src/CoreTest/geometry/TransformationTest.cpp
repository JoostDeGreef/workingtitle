#include "GoogleTest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class TransformationTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TransformationTest, NoOp)
{
    Transformation t;
    Vertex v(1, 2, 3);
    EXPECT_CONTAINER_DOUBLE_EQ(v, t * v);
}

TEST_F(TransformationTest, Translation)
{
    Transformation t({2, 4, 6});
    Vertex v(1, 2, 3);
    Vertex res(1+2, 2+4, 3+6);
    EXPECT_CONTAINER_DOUBLE_EQ(res, t * v);
}

TEST_F(TransformationTest, Rotation)
{
    Transformation r({ 0, 0, 1 }, Constants::Pi / 2);
    Vertex v(1, 1, 0);
    Vertex res(-1, 1, 0);
    EXPECT_CONTAINER_DOUBLE_EQ(res, r * v);
}

TEST_F(TransformationTest, Rotation2)
{
    Transformation r({ 0, 0, 1 }, {1, 0, 0});
    Vertex v(1, 1, 0);
    Vertex res(-1, 1, 0);
    EXPECT_CONTAINER_DOUBLE_EQ(res, r * v);
}

TEST_F(TransformationTest, RotationTranslation)
{
    Transformation t({ 2, 4, 6 });
    Transformation r({ 0, 0, 1 }, Constants::Pi / 2);
    t *= r;
    Vertex v(1, 1, 0);
    Vertex res(2-1, 4+1, 6+0);
    EXPECT_CONTAINER_DOUBLE_EQ(res, t * v);
}
