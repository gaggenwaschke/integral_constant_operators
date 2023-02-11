cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(integral_constant_operators VERSION 1.0.0 LANGUAGES CXX)

option(integral_constant_operators_CXX_STANDARD "C++ standard to use for the safe algorithm library" 20)
set(CMAKE_CXX_STANDARD 20)

add_library(integral_constant_operators INTERFACE)
target_include_directories(integral_constant_operators INTERFACE ./src)

add_subdirectory(unit_test)
