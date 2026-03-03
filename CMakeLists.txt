cmake_minimum_required(VERSION 3.12)
project(TongYiOpenai VERSION 1.0.0)


# 解决中文乱码问题
# 设置源文件编码为 UTF-8
if(MSVC)
    # 对于 Visual Studio，设置源文件使用 UTF-8 编码
    # /utf-8 选项告诉编译器将源文件和执行字符集都视为 UTF-8
    add_compile_options(/utf-8)
    # 设置执行字符集为 UTF-8（可选，/utf-8 已包含此功能）
    add_compile_definitions(_UTF8_SOURCE)
endif()


# set dirs
set(INCDIR include)
set(SRCDIR src)
set(LIBDIR lib)
set(BINDIR bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BINDIR})

set(BUILD_EXAMPLES OFF) # disable libhv examples



# src
# add_subdirectory(src)
# demo

include_directories("include/TongYiOpenAi")
add_subdirectory(demo)
# 3rd party
add_subdirectory(3rdparty/libhv)


