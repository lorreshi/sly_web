//
// Created by 石立宇 on 25-7-6.
//

#ifndef LOG_H
#define LOG_H
#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#include <iostream>

namespace sly {
    class LogLevel {
    public:
        enum Level { DEBUG = 0, INFO, WARNING, ERROR, FATAL };
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

        std::string format(LogEvent::ptr event);

    private:
    };

    /* 日志输出：抽象的日志输出接口 */
    class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender();

        virtual void log(LogLevel::Level level, const LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr formatter) {
            m_formatter = formatter;
        }
        LogFormatter::ptr getFormatter() const {
            return m_formatter;
        }

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

    /* 日志器 */
    class Logger {
    public:
        Logger(const std::string &name = "root");

        void log(LogLevel::Level level, const LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const { return m_level; }

        void setLevel(LogLevel::Level level) { m_level = level; }

    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appender;
    };

    /* 输出到控制台
     * 设置为final函数，禁止此class被继承。
     */
    class StdoutLogAppender final : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(LogLevel::Level level, const LogEvent::ptr event) override;

    private:
    };

    /* 输出到文件 */
    class FileLogAppender final : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string &filename);
        void log(LogLevel::Level level, const LogEvent::ptr event) override;

        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

    /* 输出到网络服务器 */
    class NetworkLogAppender final : public LogAppender {};

    /* 输出到数据库 */
    class DataLogAppender final : public LogAppender {};
} // namespace sly

#endif // LOG_H
