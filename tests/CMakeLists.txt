cmake_minimum_required (VERSION 3.2.2)

find_package (Threads REQUIRED)
include (ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
  gtest
  URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
  PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/3dparty/gtest
  # Disable install step
  INSTALL_COMMAND ""
)

add_library (libgtest IMPORTED STATIC GLOBAL)
add_dependencies (libgtest gtest)

ExternalProject_Get_Property(gtest source_dir binary_dir)
message (${binary_dir})
set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
  "IMPORTED_LOCATION" "${binary_dir}/libgtest_main.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories (include ../include ${source_dir}/include ../vendor/glucose/glucose-syrup)

add_executable (run_test src/main.cpp)

link_directories (${binary_dir} ../lib)
target_link_libraries (run_test Threads::Threads sat_interface ${binary_dir}/libgtest.a ${binary_dir}/libgtest_main.a)
