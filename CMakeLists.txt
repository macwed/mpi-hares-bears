cmake_minimum_required(VERSION 3.23)
project(mpi-hares-bears LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(src)
add_subdirectory(apps)

target_link_libraries(mhb_core PUBLIC cxxopts)
