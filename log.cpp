#include "log.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <cerrno>
#include <cstring>
#include "log_utils.h"

LogType Logger::loglevel_ = LogType::Info;
std::string Logger::logdir_;
std::ofstream Logger::fstream_;

Logger& Logger::Info() {
    static Logger info{LogType::Info};
    return info;
}

Logger& Logger::Error() {
    static Logger error{LogType::Error};
    return error;
}

Logger& Logger::Warn() {
    static Logger warn{LogType::Warn};
    return warn;
}

Logger::Logger(LogType type) : type_{type}, flushThreshold_{0}, lineCount_{0}, nextLine_{true} {}

int Logger::getFlushThreshold() const { return flushThreshold_; }

void Logger::setFlushThreshold(int flushThreshold) { flushThreshold_ = flushThreshold; }

std::string Logger::getLogdir() { return logdir_; }

void Logger::setLogdir(const std::string& logdir) { logdir_ = logdir; }

std::ostream& operator<<(std::ostream& strm, LogType type) {
    switch (type) {
        case LogType::Info:
            return strm << "Info:";
        case LogType::Warn:
            return strm << "Warning:";
        case LogType::Error:
            return strm << "Error:";
        default:
            return strm << "Type{" << static_cast<int>(type) << "}:";
    }
};

Logger& Logger::operator<<(const std::string& message) {
    if (!logfileIsOpen()) {
        newLogfile();
    }
    if (this->type_ >= loglevel_) {
        std::size_t pos = message.find('\n');
        if (pos != std::string::npos) {
            std::string remaining = message;
            do {
                lineCount_++;
                writeMessage(remaining.substr(0, pos + 1));
                nextLine_ = true;
                remaining = remaining.substr(pos + 1);
            } while ((pos = remaining.find('\n')) != std::string::npos);

            if (!remaining.empty()) {
                writeMessage(remaining);
            }

        } else if (!message.empty()) {
            writeMessage(message);
        }
    }
    return *this;
}

Logger& Logger::operator<<(const char* message) {
    return *this << std::string(message);
}

void Logger::writeMessage(const std::string& message) {
    if (nextLine_) {
        buffer_ << LogUtils::getDateString("-") << " " << LogUtils::getTimeString(":") << " "
                << this->type_ << " ";

        nextLine_ = false;
    }
    buffer_ << message;
    if (shouldFlush()) {
        flushBuffer();
    }
}

bool Logger::shouldFlush() const { return lineCount_ > flushThreshold_; }

bool Logger::logfileIsOpen() { return fstream_.is_open(); }

std::string Logger::newLogfile() {
    std::string filePath = (logdir_.empty() ? "" : logdir_ + "/") +
                           LogUtils::getDateString("") + LogUtils::getTimeString("") +
                           ".log.txt";
    if (logfileIsOpen()) {
        fstream_.close();
    }

    fstream_.open(filePath, std::ios::out);
    if (!fstream_.good()) {
        fstream_.close();
        throw file_open_exception(strerror(errno));
    }

    return filePath;
}

LogType Logger::getLoglevel() { return Logger::loglevel_; }

void Logger::setLoglevel(LogType loglevel) { Logger::loglevel_ = loglevel; }

void Logger::flushBuffer() {
    fstream_ << buffer_.str();
    buffer_.str(std::string());
    lineCount_ = 0;
}
