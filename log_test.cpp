//
// Created by manuel on 03.01.21.
//
#include <gtest/gtest.h>
#include <fstream>
#include "log.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
const std::string test_log_dir("test_logs");

class LogTest : public ::testing::Test {
protected:
    void SetUp() override {
        Logger::setLogdir(test_log_dir);
    }

public:
    static void SetUpTestSuite() {
        fs::create_directory(test_log_dir);
    }

    static void TearDownTestSuite() {
        fs::remove_all(test_log_dir);
    }
};


TEST_F(LogTest, newLogFileTestShouldSucceed) {
    std::string result;
    ASSERT_NO_THROW(result = Logger::newLogfile());
    ASSERT_STRNE(result.c_str(), "");
    ASSERT_FALSE(result.find(".log.txt") == std::string::npos);
}

TEST_F(LogTest, newLogFileTestShouldThrowException) {
    Logger::setLogdir("nonsense");
    ASSERT_THROW(Logger::newLogfile(), file_open_exception);
}

TEST_F(LogTest, newLogFileTestAlternating) {
    ASSERT_NO_THROW(Logger::newLogfile());
    Logger::setLogdir("nonsense");
    ASSERT_THROW(Logger::newLogfile(), file_open_exception);
    ASSERT_THROW(Logger::newLogfile(), file_open_exception);
    Logger::setLogdir("test_logs");
    ASSERT_NO_THROW(Logger::newLogfile());
    ASSERT_NO_THROW(Logger::newLogfile());
    Logger::setLogdir("nonsense");
    ASSERT_THROW(Logger::newLogfile(), file_open_exception);
}

TEST_F(LogTest, setLogDirTestShouldFindFile) {
    fs::create_directory("more_test_logs");
    Logger::setLogdir("more_test_logs");
    std::string result = Logger::newLogfile();
    ASSERT_TRUE(fs::exists(result));
    fs::remove_all("more_test_logs");

}

TEST_F(LogTest, getLogDirTestShouldMatchName) {
    std::string dirname = "some_dir_name";
    Logger::setLogdir(dirname);
    ASSERT_STREQ(Logger::getLogdir().c_str(), dirname.c_str());
}

TEST_F(LogTest, logFileOpenTestShouldBeOpen) {
    ASSERT_TRUE(Logger::logfileIsOpen());
    Logger::newLogfile();
    ASSERT_TRUE(Logger::logfileIsOpen());
}

TEST_F(LogTest, logFileOpenTestShouldBeClosed) {
    Logger::setLogdir("nonsense");
    try {
        Logger::newLogfile();
    } catch (file_open_exception& e) {

    }
    ASSERT_FALSE(Logger::logfileIsOpen());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
