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

TEST_F(ShapeTest, Constructor)
{
    Shape s;
}

TEST_F(ShapeTest, Scale)
{
    Shape s;
}

TEST_F(ShapeTest, Optimize)
{
    Shape s;
}
