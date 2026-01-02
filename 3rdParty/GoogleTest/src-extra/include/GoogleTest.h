#pragma once

#include "gtest/gtest.h"

#define EXPECT_CONTAINER_DOUBLE_EQ(ref, act) \
{\
    decltype(ref) reference = {ref}; \
    decltype(act) actual = {act}; \
    ASSERT_EQ(reference.size(), actual.size()) << "Container sizes not equal"; \
    for(size_t index = 0; index < reference.size(); ++index ) \
    {\
        EXPECT_DOUBLE_EQ(reference[index], actual[index]) << "Elements at index " << index << " don't match"; \
    }\
}


