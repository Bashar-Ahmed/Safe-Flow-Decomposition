
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(SAFE_FLOW_DECOMPOSITION)

file(GLOB LIBS ${PROJECT_SOURCE_DIR}/src/**.cpp)
file(GLOB EXECS ${PROJECT_SOURCE_DIR}/algo/**.cpp)
file(GLOB TESTS ${PROJECT_SOURCE_DIR}/test/**.cpp)

# EXECUTABLES
foreach  (EXEC IN LISTS EXECS)
    get_filename_component(ALGO ${EXEC} NAME_WE)
    add_executable(${ALGO} ${EXEC} ${LIBS})
endforeach ()

# TESTS
enable_testing()
find_package(GTest REQUIRED)
add_executable(G_TESTS ${TESTS} ${LIBS})
target_link_libraries(G_TESTS GTest::gtest_main)
gtest_discover_tests(G_TESTS)
