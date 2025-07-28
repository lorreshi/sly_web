//
// Created by 石立宇 on 25-7-22.
//
#include "log.h"

namespace sly {
    const char *LogLevel::toString(LogLevel::Level level) {
        switch (level) {
#define XX(name)                                                                                                       \
case LogLevel::name:                                                                                               \
return #name;                                                                                                  \
break;
            XX(DEBUG);
            XX(INFO);
            XX(WARNING);
            XX(ERROR);
            XX(FATAL);
#undef XX
            default:
                return "UNKNOWN";
        }
    }

    class MessageFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getContent();
        }

        ~MessageFormatItem() override = default;

        MessageFormatItem(const std::string &fmt = "") {
        };
    };

    class LevelFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << LogLevel::toString(level);
        }

        ~LevelFormatItem() override = default;

        LevelFormatItem(const std::string &fmt = "") {
        };
    };

    class ElapseFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getContent();
        }

        ~ElapseFormatItem() override = default;

        ElapseFormatItem(const std::string &fmt = "") {
        }
    };

    class NameFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << logger->getName();
        }

        ~NameFormatItem() override = default;

        NameFormatItem(const std::string &fmt = "") {
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getThreadId();
        }

        ~ThreadIdFormatItem() override = default;

        ThreadIdFormatItem(const std::string &fmt = "") {
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getFiberId();
        }

        ~FiberIdFormatItem() override = default;

        FiberIdFormatItem(const std::string &fmt = "") {
        }
    };

    class DateTimeFormatItem : public LogFormatter::FormatItem {
    public:
        DateTimeFormatItem(const std::string &format = "%Y:%m:%d %H:%M:%S") :
            m_time_pattern(format) {
            // 如果时间格式字符串为空，使用默认值 "%Y-%m-%d %H:%M:%S"。
            if (m_time_pattern.empty()) {
                m_time_pattern = "%Y-%m-%d %H:%M:%S";
            }
        }

        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            struct tm tm;
            time_t time = event->getTime();
            localtime_r(&time, &tm);
            char buffer[64];
            strftime(buffer, sizeof(buffer), m_time_pattern.c_str(), &tm);
            out << buffer;
        }

        ~DateTimeFormatItem() override = default;

    private:
        std::string m_time_pattern;
    };


    class FilenameFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getFile();
        }

        ~FilenameFormatItem() override = default;

        FilenameFormatItem(const std::string &fmt = "") {
        }
    };

    class LineFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << event->getLine();
        }

        ~LineFormatItem() override = default;

        LineFormatItem(const std::string &fmt = "") {
        }
    };

    class StringFormatItem : public LogFormatter::FormatItem {
    public:
        StringFormatItem(const std::string &str) :
            m_str(str) {
        }

        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << m_str;
        }

        ~StringFormatItem() override = default;

    private:
        std::string m_str;
    };

    class NewLineFormatItem : public LogFormatter::FormatItem {
    public:
        void format(std::ostream &out, std::shared_ptr<Logger> logger, LogLevel::Level level,
                    LogEvent::ptr event) override {
            out << std::endl;
        }

        ~NewLineFormatItem() override = default;

        NewLineFormatItem(const std::string &str = "") {
        }
    };


    Logger::Logger(const std::string &name) :
        m_name(name), m_level(LogLevel::DEBUG) {
        m_formatter.reset(new LogFormatter("%d [%p] <%f:%l>     %m %n"));
    }

    void Logger::addAppender(LogAppender::ptr appender) {
        if (!appender->getFormatter()) {
            appender->setFormatter(m_formatter);
        }
        m_appender.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender) {
        for (auto it = m_appender.begin(); it != m_appender.end(); ++it) {
            if (*it == appender) {
                m_appender.erase(it);
                break;
            }
        }
    }


    LogEvent::LogEvent(const char *file, int32_t line, uint32_t elapse, uint32_t thread_id,
                       uint32_t fiber_id, uint64_t time) :
        m_file(file), m_line(line), m_elapse(elapse), m_thread_id(thread_id),
        m_fiber_id(fiber_id), m_time(time) {
    }


    void Logger::log(LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level)
            return;
        auto self = shared_from_this();
        for (auto &i: m_appender) {
            i->log(self, level, event);
        }
    }

    void Logger::debug(LogEvent::ptr event) { log(LogLevel::DEBUG, event); }
    void Logger::info(LogEvent::ptr event) { log(LogLevel::INFO, event); }
    void Logger::warn(LogEvent::ptr event) { log(LogLevel::WARNING, event); }
    void Logger::error(LogEvent::ptr event) { log(LogLevel::ERROR, event); }
    void Logger::fatal(LogEvent::ptr event) { log(LogLevel::FATAL, event); }

    void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level)
            return;
        std::cout << m_formatter->format(logger, level, event);
    }

    void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level)
            return;
        m_filestream << m_formatter->format(logger, level, event);
    }

    bool FileLogAppender::reopen() {
        if (m_filestream.is_open()) {
            m_filestream.close();
        }
        // std::ios_base::app 以追加的方式写日志
        m_filestream.open(m_filename, std::ios_base::out | std::ios_base::app);
        return m_filestream.is_open();
    }

    LogFormatter::LogFormatter(const std::string &pattern) :
        m_format_pattern(pattern) {
        init();
    }

    std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
        std::stringstream ss;
        for (auto &i: m_format_item_list) {
            i->format(ss, logger, level, event);
        }
        return ss.str();
    }

    // 解析字符串
    void LogFormatter::init() {
        std::vector<std::tuple<std::string, std::string, int> > vec;
        std::string nstr;
        for (size_t i = 0; i < m_format_pattern.size(); ++i) {
            // 解析字符串，如果不是%插入尾部继续遍历
            if (m_format_pattern[i] != '%') {
                nstr.append(1, m_format_pattern[i]);
                continue;
            }

            if ((i + 1) < m_format_item_list.size() && m_format_pattern[i + 1] == '%') {
                // 在字符串后面追加 n 个 字符 c
                nstr.append(1, '%');
                continue;
            }

            size_t n = i + 1;
            int fmt_status = 0;
            size_t fmt_begin = 0;

            std::string str;
            std::string fmt;
            while (n < m_format_pattern.size()) {
                if (!isalpha(m_format_pattern[n]) && m_format_pattern[n] != '{'
                    && m_format_pattern[n] != '}') {
                    break;
                }
                if (fmt_status == 0) {
                    if (m_format_pattern[n] == '{') {
                        str.append(i + 1, n - i - 1);
                        fmt_status = 1;
                        fmt_begin = n;
                        ++n;
                        continue;
                    }
                }
                if (fmt_status == 1) {
                    if (m_format_pattern[n] == '}') {
                        // 返回从 pos 开始的 n 位字符串
                        fmt = str.substr(fmt_begin + 1, n - fmt_begin - 1);
                        fmt_status = 2;
                        break;
                    }
                }
                ++n;
            }

            if (fmt_status == 0) {
                if (!nstr.empty()) {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                str = m_format_pattern.substr(i + 1, n - i - 1);
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            } else if (fmt_status == 1) {
                std::cout << "parse pattern error: " << m_format_pattern << "-" << m_format_pattern.substr(i)
                        << std::endl;
                vec.push_back(std::make_tuple("pattern error", fmt, 0));
            } else if (fmt_status == 2) {
                if (!nstr.empty()) {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            }
        }

        if (!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, std::string(), 0));
        }

        static std::map<std::string, std::function<FormatItem::ptr(const std::string &str)> >
                s_format_items = {
#define FN(str, C)  \
{#str, [](const std::string& fmt) { return FormatItem::ptr (new C(fmt)); }}

                        FN(m, MessageFormatItem),
                        FN(p, LevelFormatItem),
                        FN(r, ElapseFormatItem),
                        FN(c, NameFormatItem),
                        FN(t, ThreadIdFormatItem),
                        FN(n, NewLineFormatItem),
                        FN(d, DateTimeFormatItem),
                        FN(f, FilenameFormatItem),
                        FN(l, LineFormatItem),
#undef FN
                };

        for (auto &i: vec) {
            if (std::get<2>(i) == 0) {
                m_format_item_list.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            } else {
                auto it = s_format_items.find(std::get<0>(i));
                if (it == s_format_items.end()) {
                    m_format_item_list.push_back(
                            FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                } else {
                    m_format_item_list.push_back(it->second(std::get<1>(i)));
                }
            }
            std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i)
                    << ") - (" << std::get<2>(i) << ")" << std::endl;
        }
        std::cout << m_format_item_list.size() << std::endl;
    }

} // namespace sly
