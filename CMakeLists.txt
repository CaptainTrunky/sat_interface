cmake_minimum_required (VERSION 3.2.2)

include (ExternalProject)

# Download and install GoogleTest
ExternalProject_Add (
  cppcheck
  GIT_REPOSITORY git://github.com/danmar/cppcheck.git
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cppcheck
  BUILD_COMMAND "make" "SRCDIR=build" "CFGDIR=vendor/cppcheck/src/cppcheck/cfg/" "HAVE_RULES=yes"
  # Disable install step
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property (cppcheck source_dir binary_dir)
add_custom_target (
  run_cppcheck
  COMMAND "${binary_dir}/bin/cppcheck" "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -pedantic -fpic")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native -funroll-loops -fomit-frame-pointer")

set (tools ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set (glucose_dir ${tools}/glucose-syrup)

include_directories (include ${glucose_dir})

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library (sat_interface SHARED src/SatWrapper.cpp ${glucose_dir}/simp/SimpSolver.cc ${glucose_dir}/core/Solver.cc)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/bin)

enable_testing ()
add_test (name run_test command run_test)
