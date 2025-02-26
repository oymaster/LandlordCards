cmake_minimum_required(VERSION 3.16.3)
project(test)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)
add_library(test ${SRC})
message("test: ${SRC}")
target_link_libraries(test protobuf)