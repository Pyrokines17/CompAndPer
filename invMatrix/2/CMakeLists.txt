cmake_minimum_required(VERSION 3.26)
project(evm)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(evm invMatrix2.cpp)

find_library(blas libblas.a /usr/local/lib)
MESSAGE(STATUS "CBLAS location is: " ${blas})

target_link_libraries(${PROJECT_NAME} ${blas})
