//
// Created by 石立宇 on 25-7-27.
//

#include <ios>
#include <iostream>
#include "log.h"

using namespace sly;

void test01() {
    Logger::ptr logger(new Logger());
    logger->addAppender(LogAppender::ptr(new StdoutLogAppender));
    LogEvent::ptr event(new LogEvent(__FILE__, __LINE__, 0, 1, 2, time(nullptr)));
    logger->log(LogLevel::DEBUG, event);
    event->getSS() << "sly, Hello World!";
    std::cout << "hello world" << std::endl;
}

// 日志中的获取时间戳函数相关代码API接口
void test02(const std::string& m_format) {
    // 时间日期的结构体
    struct tm tm = {0};
    const time_t t = time(nullptr);
    // 返回tm，指向本地时间日期
    localtime_r(&t, &tm);
    char buf[64];
    // 输出标准日期格式 类似于printf
    strftime(buf, sizeof(buf), m_format.c_str(), &tm);
    std::cout << buf << std::endl;
}

// 获取线程ID
#include <unistd.h>
#include <sys/syscall.h>
void test03() {
    std::cout << syscall(SYS_gettid) << std::endl;
}



int main() {
    // test01();
    test02("%Y-%m-%d %H:%M:%S");
    test02("%F %T %a %b");
    test03();
    return 0;
}
