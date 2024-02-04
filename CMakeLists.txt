cmake_minimum_required(VERSION 3.20)

project(lc3 LANGUAGES CXX)

add_library(lc3 STATIC)

target_sources(lc3 PRIVATE ./src/cpu.cpp)

target_include_directories(lc3 PUBLIC ./inc)

target_compile_features(lc3 PRIVATE cxx_std_20)

target_compile_options(lc3 PRIVATE -Werror -Wall -Wextra -pedantic)

target_compile_options(lc3 PUBLIC -g -O0 -fno-omit-frame-pointer)

add_executable(demo ./src/main.cpp)

target_compile_features(demo PRIVATE cxx_std_23)

target_compile_options(demo PRIVATE -Werror -Wall -Wextra -pedantic)

target_link_libraries(demo PRIVATE lc3)

