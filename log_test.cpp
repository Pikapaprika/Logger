//
// Created by manuel on 03.01.21.
//
#include <gtest/gtest.h>

TEST(SampleTestCase, TestWaterIsWet) {
    EXPECT_EQ("WATER", "WET");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
