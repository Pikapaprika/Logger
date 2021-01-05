//
// Created by manuel on 03.01.21.
//

#ifndef LOGGER_LOG_H
#define LOGGER_LOG_H

#include <fstream>
#include <iostream>
#include <sstream>

enum class LogType {
    Info = 0, Warn = 1, Error = 2
};

class file_open_exception : public std::runtime_error {
public:
    explicit file_open_exception(const char* message) : std::runtime_error(message) {};
};

std::ostream& operator<<(std::ostream& strm, LogType type);

class Logger {
public:
    void operator=(const Logger&) = delete;

    Logger(const Logger&) = delete;

    void operator=(Logger&&) = delete;

    Logger(Logger&&) = delete;

    static Logger& Warn();

    static Logger& Error();

    static Logger& Info();

    static LogType getLoglevel();

    static void setLoglevel(LogType loglevel);

    template<typename T>
    Logger& operator<<(const T& message) {
        static_assert(std::is_arithmetic<T>::value, "T needs to be arithmetic or std::string.");
        if (!logfileIsOpen()) {
            newLogfile();
        }
        if (this->type_ >= loglevel_) {
            writeMessage(std::to_string(message));
        }
        return *this;
    };

    Logger& operator<<(const std::string& message);

    Logger& operator<<(const char* message);

    static std::string newLogfile();

    static std::string getLogdir();

    static void setLogdir(const std::string& logdir);

    int getFlushThreshold() const;

    void setFlushThreshold(int flushThreshold);

    static bool logfileIsOpen();

private:
    explicit Logger(LogType type);
    /*
    static std::string getDateString(const std::string& separator);

    static std::string getTimeString(const std::string& separator);

    static std::string getLocaltime(const std::string& format);
    */
    static LogType loglevel_;
    static std::ofstream fstream_;
    static std::string logdir_;

    std::stringstream buffer_;
    LogType type_;
    int flushThreshold_;

    int lineCount_;
    bool nextLine_;

    void flushBuffer();

    void writeMessage(const std::string& message);

    bool shouldFlush() const;

};

#endif //LOGGER_LOG_H
