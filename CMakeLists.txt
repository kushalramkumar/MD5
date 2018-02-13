cmake_minimum_required(VERSION 2.8.9)
project (md5)

set (MD5_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCE_FILES
    md5.c
)

add_executable(md5 ${SOURCE_FILES})