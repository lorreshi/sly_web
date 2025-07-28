# 添加当前目录和C++标准库头文件路径 CMAKE_SOURCE_DIR 在这个项目中就是
# sly_web 下面的目录，即是主cmake存在的目录
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

# 设置 sly 库的源文件
set(SLY_SOURCES
                log.cc )



add_library(sly SHARED ${SLY_SOURCES})