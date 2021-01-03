#include "log.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>

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

LogType Logger::loglevel_ = LogType::Info;
std::string Logger::logdir_;
std::ofstream Logger::fstream_;

Logger::Logger(LogType type) : type_{type}, flushAfter_{1}, lineCount_{0}, nextLine_{true} {}

int Logger::flushAfter() const { return flushAfter_; }

void Logger::setFlushAfter(int flushAfter) { flushAfter_ = flushAfter; }

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
    return *this << (std::string(message));
}

void Logger::writeMessage(const std::string& message) {
    if (nextLine_) {
        buffer_ << getDateString("-") << " " << getTimeString(":") << " "
                << this->type_ << " ";

        nextLine_ = false;
    }
    buffer_ << message;
    if (shouldFlush()) {
        flushBuffer();
    }
}


bool Logger::shouldFlush() const { return lineCount_ >= flushAfter_; }

std::string Logger::getDateString(const std::string& separator) {
    std::stringstream formatStream;
    formatStream << "%Y" << separator << "%m" << separator << "%d";
    return getLocaltime(formatStream.str());
}

std::string Logger::getTimeString(const std::string& separator) {
    std::stringstream formatStream;
    formatStream << "%H" << separator << "%M" << separator << "%S";
    return getLocaltime(formatStream.str());
}

std::string Logger::getLocaltime(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    time_t now_tt = std::chrono::system_clock::to_time_t(now);
    std::stringstream timestream;
    timestream << std::put_time(std::localtime(&now_tt), format.c_str());
    return timestream.str();
}

bool Logger::logfileIsOpen() { return fstream_.is_open(); }

std::string Logger::newLogfile() {
    std::string filePath = (logdir_.empty() ? "" : logdir_ + "/") +
                           getDateString("") + getTimeString("") +
                           ".log.txt";
    if (logfileIsOpen()) {
        fstream_.close();
    }

    fstream_.open(filePath, std::ios::out);
    if (!fstream_.good()) {
        fstream_.close();
        return "";
    }

    return filePath;
}

LogType Logger::getLoglevel() { return Logger::loglevel_; }

void Logger::setLoglevel(LogType loglevel) { Logger::loglevel_ = loglevel; }

void Logger::flushBuffer() {
    // fstream_ << buffer_.str();
    std::cout << buffer_.str();
    buffer_.str(std::string());
    lineCount_ = 0;
}

// TODO: Fall-Mehrfach \n in message
// TODO: Error handling
// TODO: Nur numersche Typen u. Strings akzeptieren