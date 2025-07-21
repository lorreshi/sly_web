//
// Created by 石立宇 on 25-7-6.
//

#ifndef LOG_H
#define LOG_H
#include <cstdint>
#include <string>
#include <memory>

namespace sly {
    class LogLevel {
    public:
        enum Level {
            DEBUG = 0,
            INFO,
            WARNING,
            ERROR,
            FATAL
        };
    };

    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent();

        ~LogEvent();

    private:
        const char *m_file = nullptr;
        int32_t m_line = 0;
        int32_t m_elapse = 0; // 程序启动到现在的毫秒数
        int32_t m_threadTd = 0;
        int32_t m_fiberId = 0;
        uint64_t m_time = 0;
        std::string m_content;
    };

    class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        std::string format(const LogEvent &event);

    private:
    };

    /* 日志输出：抽象的日志输出接口 */
    class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender();

    private:
        LogLevel::Level m_level;
    };

    /* 日志器 */
    class Logger {
    public:
        Logger(const std::string &name = "root");

        ~Logger();

        void log(LogLevel::Level level, const LogEvent::ptr event);

    private:
    };

    /* 输出到控制台
     * 设置为final函数，禁止此class被继承。
     */
    class StdoutLogAppender final : public LogAppender {
    };

    /* 输出到文件 */
    class FileLogAppender final : public LogAppender {
    };

    /* 输出到网络服务器 */
    class NetworkLogAppender final : public LogAppender {
    };

    /* 输出到数据库 */
    class DataLogAppender final : public LogAppender {
    };
}

#endif //LOG_H
