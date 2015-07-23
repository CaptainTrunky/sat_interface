cmake_minimum_required (VERSION 3.2.2)

include_directories (include ../include ../vendor/gtest/include ../vendor/glucose/glucose-syrup)

add_executable (run_test src/main.cpp)

link_directories (../vendor/gtest/lib ../lib)
target_link_libraries (run_test sat_interface gtest gtest_main )
