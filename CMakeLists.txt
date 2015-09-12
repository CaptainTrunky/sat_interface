cmake_minimum_required (VERSION 3.2.2)

function (gtests_init)
  set (vendor_dir "${CMAKE_SOURCE_DIR}/vendor")
  set (gtest_dir "${vendor_dir}/gtest")

  if (NOT EXISTS ${gtest_dir})
    message ("Adding googletests lib...")

    set (git_cmd "git")
    set (git_clone "clone")
    set (gtest_url "https://github.com/CaptainTrunky/googletest.git")

    set (git_gtest "${gtest_url} ${gtest_dir}")
    message (STATUS "git cmd: ${git_cmd} ${git_clone} ${git_gtest} ${gtest_url} ${gtest_dir}")
    execute_process ( COMMAND ${git_cmd} ${git_clone} ${gtest_url} ${gtest_dir} RESULT_VARIABLE git_result)
    message (STATUS "git: ${git_result}")
  endif (NOT EXISTS ${gtest_dir})

  set (gtest_bin ${gtest_dir}/lib)

  add_subdirectory (${gtest_dir} ${gtest_bin})
endfunction (gtests_init)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -fpic")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native -funroll-loops -fomit-frame-pointer")

gtests_init ()

add_subdirectory (${CMAKE_SOURCE_DIR}/tests ${CMAKE_SOURCE_DIR}/tests/bin)

include_directories (include vendor/glucose/glucose-syrup)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
add_library (sat_interface SHARED src/SatWrapper.cpp ../glucose/glucose-syrup/simp/SimpSolver.o ../glucose/glucose-syrup/core/Solver.o)
