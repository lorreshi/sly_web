cmake_minimum_required(VERSION 3.31)
project(slyFrame)

set(CMAKE_CXX_STANDARD 23)

# 将include目录下的头文件添加到搜索目录下，全局生效，在此语句下的都可以直接调用include中的
# 头文件
include_directories(include)


# -Werror 选项，以便编译器将所有警告视为错误 # -00 禁止编译器优化
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++2a -Wall -Wno-deprecated -Werror -Wno-unused-function")
## 忽略未使用变量的警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

# 将子目录下的cmake列入主cmake构建系统
add_subdirectory(src)
set(LIB_LIB sly)
add_subdirectory(test)


