add_library(mhb_core
        core/message.cpp
        core/ticket.cpp
        clock/clock.cpp
        clock/lamport_clock.cpp
        queue/request_queue.cpp
        algorithm/algorithm.cpp
        algorithm/lamport_queue_algorithm.cpp
        policy/group_policy.cpp
        transport/transport.cpp
        transport/mpi_transport.cpp
        runtime/runtime.cpp
)

target_include_directories(mhb_core PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
