
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

project(SAFE_FLOW_DECOMPOSITION)

file(GLOB incs ${PROJECT_SOURCE_DIR}/include/**.hpp)
file(GLOB algos ${PROJECT_SOURCE_DIR}/src/**.cpp)
file(GLOB execs ${PROJECT_SOURCE_DIR}/algo/**.cpp)
file(GLOB tests ${PROJECT_SOURCE_DIR}/test/**.cpp)

add_library(library SHARED)
target_sources(library PRIVATE ${algos} PUBLIC ${incs})

# EXECUTABLES
foreach (exec IN LISTS execs)
    get_filename_component(algo ${exec} NAME_WE)
    add_executable(${algo})
    add_dependencies(${algo} library)
    target_sources(${algo} PRIVATE ${exec})
    target_link_libraries(${algo} PRIVATE library)
endforeach ()

# TESTS
enable_testing()
find_package(GTest REQUIRED)
add_executable(G_TESTS ${tests})
add_dependencies(${algo} library)
target_link_libraries(G_TESTS GTest::gtest_main library)
gtest_discover_tests(G_TESTS)
