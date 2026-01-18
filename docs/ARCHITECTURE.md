# Architecture Overview

This project is a layered, modular MPI system for distributed party formation.
It separates protocol/algorithm logic from transport, policy, and runtime so
components can evolve independently.

## High-Level Directory Structure

```
mpi-hares-bears/
├── apps/                      # Executable entry point
│   ├── mhb_main.cpp
│   └── CMakeLists.txt
├── include/mhb/               # Public headers
│   ├── algorithm/             # Algorithm interfaces + Lamport implementation
│   ├── clock/                 # Logical clock abstractions
│   ├── core/                  # Shared types, messages, config
│   ├── local/                 # Local event source simulation
│   ├── policy/                # Group formation policies
│   ├── queue/                 # Request ordering structures
│   ├── runtime/               # Event loop + orchestration
│   ├── transport/             # Transport abstraction + MPI transport
│   └── wire/                  # Serialization utilities
├── src/                       # Implementations (mirrors include/)
├── docs/                      # Documentation
│   └── ARCHITECTURE.md
└── CMakeLists.txt
```

## Current Implementation Status

Implemented components:
- Lamport logical clock.
- Request queue for ordered tickets.
- Lamport queue algorithm for forming parties.
- Lamport group policy (capacity + hare/bear weights).
- MPI transport.
- Wire protocol (byte-level serialization).
- Runtime loop + local event source.
- CLI parsing via header-only `cxxopts`.

## Layered Architecture

### 1. Application Layer (`apps/`)
**Purpose:** Entry point and configuration.

Responsibilities:
- MPI initialization/finalization.
- Parsing CLI and validating config.
- Creating transport, algorithm, and runtime.
- Starting the event loop.

### 2. Runtime + Local Layer (`runtime/`, `local/`)
**Purpose:** Orchestration and event generation.

Responsibilities:
- Main event loop: polls transport + local events.
- Dispatching events into the algorithm.
- Executing actions (`Send`, `Broadcast`, `Notify`, `Log`).
- LocalThread simulates user behavior (want party / release party).

### 3. Algorithm Layer (`algorithm/`)
**Purpose:** Distributed coordination logic.

Responsibilities:
- Handling `Request`, `Reply`, and `PartyStart`.
- Maintaining algorithm state and Lamport queue.
- Requesting groups via the policy.

Current implementation:
- `LamportQueueAlgorithm` uses total ordering and a leader-based
  party formation step.

### 4. Policy Layer (`policy/`)
**Purpose:** Domain rules for group selection.

Responsibilities:
- Enforcing capacity and composition constraints.
- Choosing a valid participant set from a candidate list.

Current implementation:
- `LamportPolicy` implements a fixed-weight hare/bear policy.

### 5. Queue Layer (`queue/`)
**Purpose:** Ordered storage of requests.

Responsibilities:
- Insert/remove tickets.
- Provide ordered snapshots.

### 6. Clock Layer (`clock/`)
**Purpose:** Logical time and ordering.

Responsibilities:
- Ticking on local events.
- Updating on message receive.

### 7. Wire Layer (`wire/`)
**Purpose:** Serialization for transport.

Responsibilities:
- Encoding messages to little-endian byte buffers.
- Decoding buffers to `Message` variants.

### 8. Transport Layer (`transport/`)
**Purpose:** Message transport abstraction.

Responsibilities:
- Sending/receiving serialized messages.
- Mapping message flow onto MPI primitives.

Current implementation:
- `MpiTransport` uses blocking `MPI_Send` and polling via `MPI_Iprobe`.

### 9. Core Types (`core/`)
**Purpose:** Shared data structures and definitions.

Contains:
- `Message` variants and tickets.
- Config and type aliases.
- Events and actions.

## Message Flow Overview

1. LocalThread generates a local event (`WantParty` or `ReleaseParty`).
2. Runtime passes the event to the algorithm.
3. Algorithm updates clock/queue and emits actions.
4. Runtime executes actions:
   - `Send`/`Broadcast` goes through transport + wire encoding.
   - `Notify` updates local state.
5. Incoming network messages are decoded and dispatched back to the algorithm.

## Design Principles

- Separation of concerns: policy, algorithm, and transport are independent.
- Local decision making: all decisions are made from replicated state.
- Replaceable modules: new clocks/algorithms/policies can be swapped in.

## Build Targets

- `mhb_core` library contains core logic and MPI transport.
- `mhb` executable wires the runtime, algorithm, and MPI startup.

