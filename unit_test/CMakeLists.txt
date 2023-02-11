cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

add_executable(integral_constant_operators_unit_test
    catch_main.cpp
    std_extensions/integral_constant_operators.cpp)
add_executable(integral_constant_operators::unit_test ALIAS integral_constant_operators_unit_test)
    
find_package(Catch2)
target_link_libraries(integral_constant_operators_unit_test PRIVATE
    integral_constant_operators
    Catch2::Catch2)
