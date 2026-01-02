#include "GoogleTest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class MatrixTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(MatrixTest, Constructor)
{
    Matrix<2, 1> m1(1,2);
    EXPECT_FLOAT_EQ(2, m1(1, 0));
    Matrix<1, 2> m2(m1.transposed());
    EXPECT_FLOAT_EQ(1, m2(0, 0));
    auto m3 = m2 * m1;
    EXPECT_EQ(2, m3.columns);
    EXPECT_EQ(2, m3.rows);
    EXPECT_FLOAT_EQ(1, m3(0, 0));
    EXPECT_FLOAT_EQ(2, m3(1, 0));
    EXPECT_FLOAT_EQ(2, m3(0, 1));
    EXPECT_FLOAT_EQ(4, m3(1, 1));
}

