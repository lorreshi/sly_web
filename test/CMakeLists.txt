# 添加当前目录和C++标准库头文件路径
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/sly)

add_executable(log_test log_test.cc)
add_dependencies(log_test sly)
target_link_libraries(log_test ${LIB_LIB})