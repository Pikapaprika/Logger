//
// Created by manuel on 03.01.21.
//
#include <gtest/gtest.h>
#include <fstream>
#include "errno.h"
#include "string.h"

TEST(SampleTestCase, TestWaterIsWet) {
    EXPECT_EQ("WATER", "WET");
}

int main(int argc, char** argv) {
    std::fstream strm("halloWelt", std::ios::in);
    std::cout << strm.bad() << strm.fail() << strm.good() << std::endl;
    if (!strm.good()) {
        std::cout << strerror(errno) << std::endl;
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
