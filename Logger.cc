#include <iostream>

#include "Timestamp.h"
#include "Logger.h"

// 获取唯一的日志实例对象
Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

// 设置日志级别
void Logger::setLogLevel(int level)
{
    logLevel_ = level;
}

// 写日志 级别信息 time：msg
void Logger::log(std::string msg)
{
    switch (logLevel_)
    {
    case INFO: /* constant-expression */
        /* code */
        std:: cout<<"[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
        break;
    case FATAL:
        std::cout << "[FATAL]";
        break;
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }

    // print time and msg
       std::cout << Timestamp::now().toString() << " : " << msg << std::endl;
}