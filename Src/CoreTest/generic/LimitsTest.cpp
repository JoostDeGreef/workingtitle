#include "gtest/gtest.h"

#include "Core.h"

using namespace std;
using namespace testing;

class LimitsTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(LimitsTest, Values)
{
    EXPECT_GT(Limits<double>::MaxValue, Limits<double>::MinValue);
    EXPECT_GT(Limits<float>::MaxValue, Limits<float>::MinValue);
    EXPECT_GT(Limits<double>::MaxValue, Limits<float>::MaxValue);
    EXPECT_EQ(1, Limits<int>::CompareEpsilon);
}

