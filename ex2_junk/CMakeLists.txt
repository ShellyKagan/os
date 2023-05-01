#cmake_minimum_required(VERSION 3.24)
#project(ex2_junk)
#
#set(CMAKE_CXX_STANDARD 11)
#
#add_executable(ex2_junk main.cpp)
cmake_minimum_required(VERSION 3.1)
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
project(threads VERSION 1.0 LANGUAGES C CXX)

add_library(uthreads uthreads.h uthreads.cpp ready_queue.cpp ready_queue.h sleeping_list.cpp sleeping_list.h)

set_property(TARGET uthreads PROPERTY CXX_STANDARD 11)
target_compile_options(uthreads PUBLIC -Wall -Wextra)

add_subdirectory(tests)