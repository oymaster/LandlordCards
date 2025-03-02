# 设置 CMake 的最低版本要求为 3.16
cmake_minimum_required(VERSION 3.16)

# 定义项目名称为 crypto
project(crypto)


# 将当前目录下所有源文件列入变量 SRC
# 'aux_source_directory' 是一个用于自动查找指定目录下所有源文件的命令
aux_source_directory(./ SRC)
# 显示头文件
file(GLOB HEAD ./*.h)

#将当前目录下的源文件打包成静态库
add_library(crypto ${SRC} ${HEAD})

# 链接系统Core动态库
target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core
    "D:\\code\\OpenSSL-Win64\\lib\\VC\\x64\\MD\\libcrypto.lib"
)
