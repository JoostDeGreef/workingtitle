#include "gtest/gtest.h"
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
    Matrix<2,1> m1(1,2);
    EXPECT_FLOAT_EQ(2, m1(1, 0));
    Matrix<1, 2> m2(m1.transposed());
    EXPECT_FLOAT_EQ(1, m2(0, 0));
    auto m3 = m1 * m2;
    EXPECT_EQ(2, m3.columns);
    EXPECT_EQ(2, m3.rows);
}

