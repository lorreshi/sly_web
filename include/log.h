//
// Created by 石立宇 on 25-7-6.
//

#ifndef LOG_H
#define LOG_H
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <strstream>
#include <vector>
#include <functional>
#include <map>
#include <time.h>

namespace sly {
    class Logger;

    class LogLevel {
    public:
        enum Level { UNKNOW = 0, DEBUG, INFO, WARNING, ERROR, FATAL };

        static const char *toString(LogLevel::Level level);
    };

    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent();

        LogEvent(const char *file, int32_t line, uint32_t elapse, uint32_t thread_id,
                 uint32_t fiber_id, uint64_t time);

        const char *getFile() const { return m_file; }
        uint32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_thread_id; }
        uint32_t getFiberId() const { return m_fiber_id; }
        uint64_t getTime() const { return m_time; }
        std::string getContent() const { return m_ss.str(); }
        std::stringstream &getSS() { return m_ss; };

    private:
        const char *m_file = nullptr;
        uint32_t m_line = 0; // 行号
        uint32_t m_elapse = 0; // 程序启动到现在的毫秒数
        uint32_t m_thread_id = 0; // 线程ID
        uint32_t m_fiber_id = 0; // 协程ID
        uint64_t m_time = 0; // 时间
        std::stringstream m_ss; //
    };

    /** 日志格式类
     *
     */
    class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        explicit LogFormatter(const std::string &pattern);

        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

        void init();

    public:
        class FormatItem {
        public:
            typedef std::shared_ptr<FormatItem> ptr;

            virtual ~FormatItem() {
            }

            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level,
                                LogEvent::ptr event) = 0;
        };

    private:
        // 输入的格式化字符串
        std::string m_format_pattern;
        // 格式化完毕解析的vec列表
        std::vector<FormatItem::ptr> m_format_item_list;
    };

    /* 日志输出：抽象的日志输出接口 */
    class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender() = default;

        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }
        LogFormatter::ptr getFormatter() const { return m_formatter; }

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

    /* 日志器 */
    class Logger : public std::enable_shared_from_this<Logger> {
    public:
        typedef std::shared_ptr<Logger> ptr;

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
        const std::string &getName() const { return m_name; }

    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appender;
        LogFormatter::ptr m_formatter;
    };

    /* 输出到控制台
     * 设置为final函数，禁止此class被继承。
     */
    class StdoutLogAppender final : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) override;

    private:
    };

    /* 输出到文件 */
    class FileLogAppender final : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;

        FileLogAppender(const std::string &filename);

        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) override;

        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

    /* 输出到网络服务器 */
    // class NetworkLogAppender final : public LogAppender {};

    /* 输出到数据库 */
    // class DataLogAppender final : public LogAppender {};
} // namespace sly

#endif // LOG_H
