cmake_minimum_required(VERSION 3.16.3)
project(serialize)
set(CMAKE_CXX_STANDARD 17)
#取出当前源文件，存储到一个变量
aux_source_directory(./ SRC)
#制作成静态库
add_library(serialize STATIC ${SRC})

