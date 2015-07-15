cmake_minimum_required (VERSION 3.2.2)

set (CMAKE_CXX_FLAGS "-std=c++11 -Ofast -march=native")

include_directories (include vendor/glucose/glucose-syrup)
add_executable (sat src/main.cpp)
