# 设置 CMake 的最低版本要求为 3.16
cmake_minimum_required(VERSION 3.16)

# 定义项目名称为 'windows'
project(windows)


file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
aux_source_directory(./ SRC)

message("dir1 = ${HEADERS}")

# 将当前目录下所有源文件列入变量 SRC
# 'aux_source_directory' 是一个用于自动查找指定目录下所有源文件的命令


# 创建一个名为 'windows' 的库，并将 SRC 中的所有源文件作为该库的文件
# 这里的 SRC 包含了通过 aux_source_directory 命令收集到的所有源文件
add_library(windows ${SRC} ${HEADERS} ${UIS})

# 链接系统Core动态库
target_link_libraries(windows PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)
