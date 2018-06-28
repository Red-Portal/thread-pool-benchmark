
cmake_minimum_required(VERSION 3.5)
project(thread-pool-benchmark)

# project source files
set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/cpp_threads/cpp_threads.cpp
    #${PROJECT_SOURCE_DIR}/sean_parent/sean_parent_naive.cpp
    #${PROJECT_SOURCE_DIR}/sean_parent/sean_parent_multiqueue.cpp
    #${PROJECT_SOURCE_DIR}/sean_parent/sean_parent_worksteal.cpp
    ${PROJECT_SOURCE_DIR}/sean_parent/sean_parent.cpp
    ${PROJECT_SOURCE_DIR}/benchmark_problems/matrix_multiplication.cpp)

message("[ Source files for ${PROJECT_NAME} ]")
foreach(SOURCE_FILE ${SOURCE_FILES})
    message(STATUS "${SOURCE_FILE}")
endforeach()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Build options
if(MSVC)
    set(BUILD_FLAGS
	"${CMAKE_CXX_FLAGS} /W3 /O2")
else()
    set(BUILD_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++14 -g -O3 -Wall -Wextra -Wpedantic")
endif()

find_package (Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

message("C++ flags = ${BUILD_FLAGS}")
