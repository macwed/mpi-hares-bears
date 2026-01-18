#include "mhb/wire/wire_protocol.hpp"

#include "mhb/wire/byte_io.hpp"

namespace mhb::wire {
namespace {
enum class WireType : std::uint8_t {
  kRequest = 1,
  kReply = 2,
  kPartyStart = 3,
};
}  // namespace

std::vector<std::uint8_t> Serialize(const Message& msg) {
  ByteWriter w;

  std::visit([&](const auto& m) {
    using T = std::decay_t<decltype(m)>;

    if constexpr (std::is_same_v<T, Request>) {
      w.byte(static_cast<std::uint8_t>(WireType::kRequest));
      w.u<Pid>(m.request_ticket.pid);
      w.u<Timestamp>(m.request_ticket.ts);
    } else if constexpr (std::is_same_v<T, Reply>) {
      w.byte(static_cast<std::uint8_t>(WireType::kReply));
      w.u<Pid>(m.sender_ticket.pid);
      w.u<Timestamp>(m.sender_ticket.ts);
      w.u<Pid>(m.request_ticket.pid);
      w.u<Timestamp>(m.request_ticket.ts);
    } else if constexpr (std::is_same_v<T, PartyStart>) {
      w.byte(static_cast<std::uint8_t>(WireType::kPartyStart));
      w.u<Pid>(m.sender_ticket.pid);
      w.u<Timestamp>(m.sender_ticket.ts);
      w.u<PartyId>(m.party_id);

      const auto count = static_cast<std::uint32_t>(m.participants.size());
      w.u<std::uint32_t>(count);
      for (Pid pid : m.participants) {
        w.u<Pid>(pid);
      }
    }
  }, msg);

  return w.take();
}

std::optional<Message> Deserialize(const std::vector<std::uint8_t>& data) {
  ByteReader r(data);

  std::uint8_t type_u8 = 0;
  if (!r.byte(type_u8)) return std::nullopt;

  const auto type = static_cast<WireType>(type_u8);
  switch (type) {
    case WireType::kRequest: {
      Pid pid = 0;
      Timestamp ts = 0;
      if (!r.u<Pid>(pid) || !r.u<Timestamp>(ts)) return std::nullopt;
      return Message{Request{Ticket{pid, ts}}};
    }
    case WireType::kReply: {
      Pid sp = 0;
      Timestamp sts = 0;
      Pid rp = 0;
      Timestamp rts = 0;
      if (!r.u<Pid>(sp) || !r.u<Timestamp>(sts) ||
          !r.u<Pid>(rp) || !r.u<Timestamp>(rts)) return std::nullopt;
      return Message{Reply{Ticket{sp, sts}, Ticket{rp, rts}}};
    }
    case WireType::kPartyStart: {
      Pid sp = 0;
      Timestamp sts = 0;
      PartyId party_id = 0;
      std::uint32_t count = 0;
      if (!r.u<Pid>(sp) || !r.u<Timestamp>(sts) ||
          !r.u<PartyId>(party_id) || !r.u<std::uint32_t>(count)) {
        return std::nullopt;
      }

      if (count > 1'000'000u) return std::nullopt;

      std::vector<Pid> participants;
      participants.reserve(count);
      for (std::uint32_t i = 0; i < count; ++i) {
        Pid pid = 0;
        if (!r.u<Pid>(pid)) return std::nullopt;
        participants.push_back(pid);
      }

      return Message{PartyStart{Ticket{sp, sts}, party_id, std::move(participants)}};
    }
  }

  return std::nullopt;
}
}  // namespace mhb::wire
