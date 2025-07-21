//
// Created by 石立宇 on 25-7-22.
//
#include "log.h"

namespace sly {

    Logger::Logger(const std::string &name) : m_name(name) {}

    void Logger::addAppender(LogAppender::ptr appender) {
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

    void Logger::log(LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level) return;
        for (auto& i : m_appender) {
            i->log(level, event);
        }
    }

    void Logger::debug(LogEvent::ptr event) {
        log(LogLevel::DEBUG, event);
    }
    void Logger::info(LogEvent::ptr event) {
        log(LogLevel::INFO, event);
    }
    void Logger::warn(LogEvent::ptr event) {
        log(LogLevel::WARNING, event);
    }
    void Logger::error(LogEvent::ptr event) {
        log(LogLevel::ERROR, event);
    }
    void Logger::fatal(LogEvent::ptr event) {
        log(LogLevel::FATAL, event);
    }

    void StdoutLogAppender::log(LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level) return;
        std::cout << m_formatter->format(event);
    }

    void FileLogAppender::log(LogLevel::Level level, const LogEvent::ptr event) {
        if (level < m_level) return;
        m_filestream << m_formatter->format(event);
    }

    bool FileLogAppender::reopen() {
        if (m_filestream.is_open()) {
            m_filestream.close();
        }
        m_filestream.open(m_filename);
        return m_filestream.is_open();
    }


} // namespace sly
