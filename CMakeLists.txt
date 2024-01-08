cmake_minimum_required(VERSION 3.20)

project(lc3 LANGUAGES CXX)

add_executable(lc3)

target_sources(lc3 PRIVATE ./src/cpu.cpp)

target_include_directories(lc3 PRIVATE ./inc)

target_compile_features(lc3 PRIVATE cxx_std_20)

target_compile_options(lc3 PRIVATE -Werror -Wall -Wextra -pedantic)

