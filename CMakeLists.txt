cmake_minimum_required(VERSION 3.31)
project(slyFrame)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

# -Werror 选项，以便编译器将所有警告视为错误 # -00 禁止编译器优化
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++2a -Wall -Wno-deprecated -Werror -Wno-unused-function")
## 忽略未使用变量的警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")


add_subdirectory(src)


add_executable(main main.cpp)

