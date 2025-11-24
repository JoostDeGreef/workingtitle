#include "gtest/gtest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class NumericsTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(NumericsTest, Sqr)
{
    EXPECT_FLOAT_EQ(9.0, Numerics::Sqr(3.0));
    EXPECT_FLOAT_EQ(9.0f, Numerics::Sqr(3.0f));
}
