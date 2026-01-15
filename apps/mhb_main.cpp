#include <mpi.h>

#include <cstdint>
#include <memory>

#include "mhb/algorithm/lamport_algo/lamport_queue_algorithm.hpp"
#include "mhb/core/config.hpp"
#include "mhb/local/local_thread.hpp"
#include "mhb/runtime/runtime.hpp"
#include "mhb/transport/mpi_transport.hpp"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  mhb::config::Config cfg;
  cfg.world_size = static_cast<uint32_t>(size);

  auto transport = std::make_unique<mhb::MpiTransport>();
  auto algorithm = std::make_unique<mhb::LamportQueueAlgorithm>(
      static_cast<mhb::Pid>(rank), cfg);
  mhb::LocalThread local_src;

  mhb::Runtime runtime(std::move(transport), std::move(algorithm), std::move(local_src));
  runtime.Run();

  MPI_Finalize();
  return 0;
}
