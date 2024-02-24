
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fno-rtti -Wall -Wextra -O3")

project(SAFE_FLOW_DECOMPOSITION)

include_directories(include)
file(GLOB algos ${PROJECT_SOURCE_DIR}/src/**.cpp)
file(GLOB execs ${PROJECT_SOURCE_DIR}/algo/**.cpp)
file(GLOB tests ${PROJECT_SOURCE_DIR}/test/**.cpp)

add_library(src SHARED)
target_sources(src PRIVATE ${algos})

# EXECUTABLES
foreach (exec IN LISTS execs)
    get_filename_component(algo ${exec} NAME_WE)
    add_executable(${algo})
    add_dependencies(${algo} src gtest)
    target_sources(${algo} PRIVATE ${exec})
    target_link_libraries(${algo} PRIVATE src)
    add_test(${algo} ./gtest --gtest_filter=*${algo}*)
endforeach ()

# GTEST
find_package(GTest REQUIRED)
add_executable(gtest ${tests})
add_dependencies(${algo} src)
target_link_libraries(gtest src GTest::gtest_main)

# CTEST
enable_testing()
add_test(all ./gtest)
set(datasets zebrafish mouse human salmon refsim simulation)
foreach (dataset IN LISTS datasets)
    add_test(${dataset} ./gtest --gtest_filter=*${dataset}*)
endforeach()
