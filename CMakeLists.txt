
cmake_minimum_required(VERSION 3.5)
project(thread-pool-benchmark)

option(WITH_CPP_THREADS "Run benchmark with c++11 threads" ON)
#option(WITH_HPX "Run benchmark with HPX" ON)
option(WITH_GCD "Run benchmark with Grand Central Dispatch" ON)
option(WITH_TBB "Run benchmark with Intel TBB" ON)

# cmake modules path
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/benchmark_problems/matrix_multiplication.cpp
    ${PROJECT_SOURCE_DIR}/benchmark_problems/fluid_solver.cpp)

set(BENCHMARK_CANDIDATES_DIR ${PROJECT_SOURCE_DIR}/benchmark_candidates)

if(WITH_CPP_THREADS)
    set(SOURCE_FILES ${SOURCE_FILES}
	${BENCHMARK_CANDIDATES_DIR}/cpp_threads/cpp_threads.cpp
	${BENCHMARK_CANDIDATES_DIR}/sean_parent/sean_parent.cpp
	${BENCHMARK_CANDIDATES_DIR}/progschj_ThreadPool/progschj_ThreadPool.cpp
	${BENCHMARK_CANDIDATES_DIR}/thread-pool-cpp/thread-pool-cpp.cpp)
endif()
if(WITH_GCD)
    set(SOURCE_FILES ${SOURCE_FILES}
	${BENCHMARK_CANDIDATES_DIR}/grand_central_dispatch/grand_central_dispatch.cpp)
endif()
# if(WITH_HPX)
#     set(SOURCE_FILES ${SOURCE_FILES}
# 	${BENCHMARK_CANDIDATES_DIR}/hpx/hpx.cpp)
# endif()
if(WITH_TBB)
    set(SOURCE_FILES ${SOURCE_FILES}
	${BENCHMARK_CANDIDATES_DIR}/thread_building_blocks/thread_building_blocks.cpp)
endif()

message("[ Source files for ${PROJECT_NAME} ]")
foreach(SOURCE_FILE ${SOURCE_FILES})
    message(STATUS "${SOURCE_FILE}")
endforeach()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(thread-pool-cpp ${BENCHMARK_CANDIDATES_DIR}/thread-pool-cpp/thread-pool-cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${thread-pool-cpp}/include)

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

if(WITH_GCD)
    if (NOT("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
	message(FATAL_ERROR "Grand Central Dispatch requires clang as compiler.")
    endif()

    find_package(dispatch REQUIRED)
    target_include_directories(${PROJECT_NAME}
	PUBLIC ${DISPATCH_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}
	${DISPATCH_LIBRARIES})
endif()
# if(WITH_HPX)
#     find_package(HPX REQUIRED)
#     add_definitions(-DWITH_HPX)
#     target_include_directories(${PROJECT_NAME}
# 	PUBLIC ${HPX_INCLUDE_DIRS})
#     target_link_libraries(${PROJECT_NAME}
# 	${HPX_LIBRARIES})
# endif()
if(WITH_TBB)
    find_package(TBB REQUIRED)
    target_include_directories(${PROJECT_NAME}
	PUBLIC ${TBB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}
	${TBB_LIBRARIES})
endif()


message("C++ flags = ${BUILD_FLAGS}")
