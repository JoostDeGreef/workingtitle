#include "GoogleTest.h"
#include "Core.h"

using namespace std;
using namespace testing;

class FaceTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(FaceTest, Constructor)
{
    Face f1;
    EXPECT_EQ(0, f1.size());
    Face f2({ 1,2,3,4 });
    EXPECT_EQ(4, f2.size());
    Face f3({ 1,2,3,4,5 });
    EXPECT_EQ(5, f3.size());
}
