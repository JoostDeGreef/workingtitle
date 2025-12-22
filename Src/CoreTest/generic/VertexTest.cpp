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
    Vertex v1(1,2,3);
    EXPECT_FLOAT_EQ(1.0, v1.x);
    EXPECT_FLOAT_EQ(2.0, v1.y);
    EXPECT_FLOAT_EQ(3.0, v1.z);
    Vertex v2(v1);
    EXPECT_FLOAT_EQ(1.0, v2.x);
    EXPECT_FLOAT_EQ(2.0, v2.y);
    EXPECT_FLOAT_EQ(3.0, v2.z);
}

TEST_F(VertexTest, Addition)
{
    Vertex v1(1, 2, 3);
    Vertex v2 = v1 + v1;
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
    Vertex v1(3, 6, 9);
    Vertex v2(1, 2, 3);
    Vertex v3 = v1 - v2;
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
    Vertex v1(1, 2, 3);
    Vertex v2(1, 2, 3);
    Vertex v3(1, 2, 3 + 2*Limits<Scalar>::CompareEpsilon);
    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}

TEST_F(VertexTest, Indexing)
{
    Vertex v(1, 2, 3);
    EXPECT_EQ(2, v[1]);
    EXPECT_THROW(v[3], std::invalid_argument);
}
