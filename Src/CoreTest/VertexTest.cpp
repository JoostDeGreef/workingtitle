#include "gtest/gtest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class VertexTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(VertexTest, Constructor)
{
    Vertex<double> v1(1,2,3);
    EXPECT_FLOAT_EQ(1.0, v1.x);
    EXPECT_FLOAT_EQ(2.0, v1.y);
    EXPECT_FLOAT_EQ(3.0, v1.z);
    Vertex<double> v2(v1);
    EXPECT_FLOAT_EQ(1.0, v2.x);
    EXPECT_FLOAT_EQ(2.0, v2.y);
    EXPECT_FLOAT_EQ(3.0, v2.z);
}

TEST_F(VertexTest, Addition)
{
    Vertex<double> v1(1, 2, 3);
    Vertex<double> v2 = v1 + v1;
    EXPECT_FLOAT_EQ(2.0, v2.x);
    EXPECT_FLOAT_EQ(4.0, v2.y);
    EXPECT_FLOAT_EQ(6.0, v2.z);
    v2 += v1;
    EXPECT_FLOAT_EQ(3.0, v2.x);
    EXPECT_FLOAT_EQ(6.0, v2.y);
    EXPECT_FLOAT_EQ(9.0, v2.z);
}

TEST_F(VertexTest, Subtraction)
{
    Vertex<double> v1(3, 6, 9);
    Vertex<double> v2(1, 2, 3);
    Vertex<double> v3 = v1 - v2;
    EXPECT_FLOAT_EQ(2.0, v3.x);
    EXPECT_FLOAT_EQ(4.0, v3.y);
    EXPECT_FLOAT_EQ(6.0, v3.z);
    v3 -= v2;
    EXPECT_FLOAT_EQ(1.0, v3.x);
    EXPECT_FLOAT_EQ(2.0, v3.y);
    EXPECT_FLOAT_EQ(3.0, v3.z);
}

TEST_F(VertexTest, Equality)
{
    Vertex<double> v1(1, 2, 3);
    Vertex<double> v2(1, 2, 3);
    Vertex<double> v3(1, 2, 3 + 2*Limits<double>::CompareEpsilon);
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}
