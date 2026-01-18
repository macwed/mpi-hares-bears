cmake_minimum_required(VERSION 3.23)
project(mpi-hares-bears LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)

include(FetchContent)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(cxxopts)

target_link_libraries(mhb_core PUBLIC cxxopts)
