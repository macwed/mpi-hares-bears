cmake_minimum_required(VERSION 3.23)
project(mpi-hares-bears LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)
