#include "mhb/transport/mpi_transport.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

#include <mpi.h>

#include "mhb/wire/wire_protocol.hpp"

namespace mhb {
namespace {
constexpr int kMsgTag = 1;
}  // namespace

MpiTransport::MpiTransport() {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  self_ = static_cast<Pid>(rank);
  world_size_ = static_cast<uint32_t>(size);
}

void MpiTransport::Send(Pid to, const Message& msg) {
  const auto payload = wire::Serialize(msg);
  MPI_Send(payload.data(), static_cast<int>(payload.size()), MPI_BYTE,
           static_cast<int>(to), kMsgTag, MPI_COMM_WORLD);
}

void MpiTransport::Broadcast(const Message& msg) {
  const auto payload = wire::Serialize(msg);
  for (Pid pid = 0; pid < world_size_; ++pid) {
    if (pid == self_) {
      continue;
    }
    MPI_Send(payload.data(), static_cast<int>(payload.size()), MPI_BYTE,
             static_cast<int>(pid), kMsgTag, MPI_COMM_WORLD);
  }
}

std::vector<Net> MpiTransport::Poll() {
  std::vector<Net> events;
  int flag = 0;
  MPI_Status status;
  while (true) {
    MPI_Iprobe(MPI_ANY_SOURCE, kMsgTag, MPI_COMM_WORLD, &flag, &status);
    if (!flag) {
      break;
    }
    int count = 0;
    MPI_Get_count(&status, MPI_BYTE, &count);
    std::vector<std::uint8_t> buffer(static_cast<size_t>(count));
    MPI_Recv(buffer.data(), count, MPI_BYTE, status.MPI_SOURCE, kMsgTag,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    auto msg = wire::Deserialize(buffer);
    if (!msg) {
      continue;
    }
    events.push_back(Net{static_cast<Pid>(status.MPI_SOURCE), std::move(*msg)});
  }
  return events;
}

Pid MpiTransport::Self() const {
  return self_;
}

uint32_t MpiTransport::WorldSize() const {
  return world_size_;
}
}  // namespace mhb
