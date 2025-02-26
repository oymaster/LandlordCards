# 设置 CMake 的最低版本要求为 3.16
cmake_minimum_required(VERSION 3.16)

# 定义项目名称为 'player'
project(player)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)

# 将当前目录下所有源文件列入变量 SRC
# 'aux_source_directory' 是一个用于自动查找指定目录下所有源文件的命令
aux_source_directory(./ SRC)

# 创建一个名为 'card' 的库，并将 SRC 中的所有源文件作为该库的文件
# 这里的 SRC 包含了通过 aux_source_directory 命令收集到的所有源文件
add_library(player ${SRC} ${HEADERS})


# 链接系统Core动态库
target_link_libraries(player PRIVATE Qt${QT_VERSION_MAJOR}::Core)
