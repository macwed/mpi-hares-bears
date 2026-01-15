add_library(mhb_core
)

target_sources(mhb_core PRIVATE
        ${PROJECT_SOURCE_DIR}/src/core/message.cpp
        ${PROJECT_SOURCE_DIR}/src/core/ticket.cpp
        ${PROJECT_SOURCE_DIR}/src/clock/clock.cpp
        ${PROJECT_SOURCE_DIR}/src/clock/lamport_clock.cpp
        ${PROJECT_SOURCE_DIR}/src/queue/request_queue.cpp
        ${PROJECT_SOURCE_DIR}/src/algorithm/lamport_queue_algorithm.cpp
        ${PROJECT_SOURCE_DIR}/src/policy/lamport_policy/lamport_policy.cpp
        ${PROJECT_SOURCE_DIR}/src/transport/mpi_transport.cpp
        ${PROJECT_SOURCE_DIR}/src/wire/wire_protocol.cpp
        ${PROJECT_SOURCE_DIR}/src/runtime/runtime.cpp
)

target_include_directories(mhb_core PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_sources(mhb_core PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES
        ${PROJECT_SOURCE_DIR}/include/mhb/core/action.hpp
        ${PROJECT_SOURCE_DIR}/include/mhb/algorithm/state.hpp
)

target_link_libraries(mhb_core PUBLIC MPI::MPI_CXX)
