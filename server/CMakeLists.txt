cmake_minimum_required(VERSION 3.16.3)
project(server)

set(CMAKE_CXX_STANDARD 17)

#添加头文件目录
include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
        ${PROJECT_SOURCE_DIR}/test
)

add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)
#测试用子目录
add_subdirectory(test)


#链接子目录静态库文件
link_libraries(tcp thread http reactor common test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(server main.cpp)
