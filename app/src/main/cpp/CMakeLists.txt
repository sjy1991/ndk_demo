
cmake_minimum_required(VERSION 3.4.1)

# 设置头文件目录
include_directories(${CMAKE_SOURCE_DIR}/bzip2)
# 设置源文件目录
aux_source_directory(${CMAKE_SOURCE_DIR}/bzip2 bzip2_src)

add_library( # Sets the name of the library.
        native-lib
        SHARED
        bspatch.c
        ${bzip2_src}
        native-lib.cpp)



target_link_libraries( # Specifies the target library.
        native-lib
        log)