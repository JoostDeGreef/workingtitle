#include "gtest/gtest.h"

#include "Core.h"

using namespace std;
using namespace testing;

class StackAllocatorTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(StackAllocatorTest, Buffer)
{
    StackAllocatorBuffer<100> b;
    int end = b.blocks;
    // test for empty list with only start block
    EXPECT_EQ(std::vector<int>({ 0, end }), b.getBlocks());
    // test for list with one block
    auto p0 = b.allocate(1);
    EXPECT_EQ(std::vector<int>({ 0, end-2, -end }), b.getBlocks());
    // release only block, empty list again
    b.deallocate(p0, 1);
    EXPECT_EQ(std::vector<int>({ 0, end }), b.getBlocks());
    // allocate 2 blocks
    auto p1 = b.allocate(1);
    auto p2 = b.allocate(1);
    EXPECT_EQ(std::vector<int>({ 0, end - 4, -(end - 2), -end }), b.getBlocks());
    // leave one block unused
    b.deallocate(p1, 1);
    EXPECT_EQ(std::vector<int>({ 0, end - 4, -(end - 2), end }), b.getBlocks());
    // release last block, empty list again
    b.deallocate(p2, 1);
    EXPECT_EQ(std::vector<int>({ 0, end }), b.getBlocks());
}

template<size_t SIZE>
class StackMemoryWrapper : public StackMemory<SIZE>
{
public:
    std::vector<int> getBlocks()
    {
        return buffer.getBlocks();
    }
};

TEST_F(StackAllocatorTest, StackMemory)
{
    StackMemoryWrapper<100> sm;
    auto v0 = sm.vector<int>();
    auto v1 = sm.vector<double>();
    auto v2 = sm.vector<int>(2);
    auto v3 = sm.vector<int>(5, 1);

    v1.push_back(1);
    v2.push_back(1);
    v1.push_back(1);
    v2.push_back(1);
    v1.push_back(1);
    v2.clear();
}

