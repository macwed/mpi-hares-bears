#include <mpi.h>

#include <iostream>
#include <memory>

#include <cxxopts.hpp>

#include "mhb/algorithm/lamport_algo/lamport_queue_algorithm.hpp"
#include "mhb/core/config.hpp"
#include "mhb/local/local_thread.hpp"
#include "mhb/runtime/runtime.hpp"
#include "mhb/transport/mpi_transport.hpp"

static bool ApplyCli(int argc, char** argv, mhb::config::Config& cfg,
                     uint32_t mpi_size, int rank) {
  try {
    cxxopts::Options options("mhb", "MPI Hares & Bears");

    options.add_options()
      ("world-size", "Expected MPI world size (validation only)",
        cxxopts::value<uint32_t>())
      ("party-capacity", "Party capacity",
        cxxopts::value<uint32_t>()->default_value(std::to_string(cfg.party_capacity)))
      ("num-bears", "Number of bears (hares = world_size - bears)",
        cxxopts::value<uint32_t>()->default_value(std::to_string(cfg.num_bears)))
      ("h,help", "Print help");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      if (rank == 0) std::cout << options.help() << "\n";
      return false;
    }

    if (result.count("world-size")) {
      auto expected = result["world-size"].as<uint32_t>();
      if (expected != mpi_size) {
        if (rank == 0) {
          std::cerr << "Config error: expected world-size=" << expected
                    << " but MPI has " << mpi_size
                    << " (use mpirun -np " << expected << ")\n";
        }
        MPI_Abort(MPI_COMM_WORLD, 2);
      }
    }

    cfg.world_size = mpi_size; // MPI prawdę powie
    cfg.party_capacity = result["party-capacity"].as<uint32_t>();
    cfg.num_bears = result["num-bears"].as<uint32_t>();

    if (cfg.party_capacity == 0) {
      if (rank == 0) std::cerr << "Config error: party-capacity must be > 0\n";
      MPI_Abort(MPI_COMM_WORLD, 2);
    }
    if (cfg.num_bears > cfg.world_size) {
      if (rank == 0) std::cerr << "Config error: num-bears must be <= world_size\n";
      MPI_Abort(MPI_COMM_WORLD, 2);
    }

    cfg.num_hares = cfg.world_size - cfg.num_bears;
    if (cfg.num_hares == 0) {
      if (rank == 0) std::cerr << "Config error: computed num_hares is 0; lower num-bears\n";
      MPI_Abort(MPI_COMM_WORLD, 2);
    }

    return true;
  } catch (const std::exception& e) {
    if (rank == 0) std::cerr << "CLI parse error: " << e.what() << "\n";
    MPI_Abort(MPI_COMM_WORLD, 2);
  }
  return false;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank = 0, size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  mhb::config::Config cfg;
  if (!ApplyCli(argc, argv, cfg, static_cast<uint32_t>(size), rank)) {
    MPI_Finalize();
    return 0;
  }

  auto transport = std::make_unique<mhb::MpiTransport>();
  auto algorithm = std::make_unique<mhb::LamportQueueAlgorithm>(
      static_cast<mhb::Pid>(rank), cfg);
  mhb::LocalThread local_src;

  mhb::Runtime runtime(std::move(transport), std::move(algorithm), local_src);
  runtime.Run();

  MPI_Finalize();
  return 0;
}
