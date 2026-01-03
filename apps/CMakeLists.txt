add_executable(mhb mhb_main.cpp)

target_link_libraries(mhb PRIVATE mhb_core MPI::MPI_CXX)
