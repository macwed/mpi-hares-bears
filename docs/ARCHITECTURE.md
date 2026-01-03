# Architecture Overview

This project is structured as a **layered, modular distributed system**, explicitly separating:
- synchronization algorithms,
- logical clocks and ordering,
- transport and runtime,
- domain-specific policies.

The architecture is designed to support **multiple interchangeable distributed algorithms**
(e.g. Lamport-based, gossip-based, DAG-based) without rewriting application logic.

---

## High-Level Directory Structure

```
mpi-hares-bears/
├── apps/ # Executables / entry points
│   ├── mhb_main.cpp
│   └── CMakeLists.txt
├── README.md
├── README_PL.md
├── include/
│   └── mhb/
│       ├── algorithm/ # Synchronization algorithm interfaces & implementations
│       ├── clock/ # Logical clock abstractions
│       ├── core/ # Core shared data types
│       ├── policy/ # Group formation policies
│       ├── queue/ # Request ordering structures
│       ├── runtime/ # Event loop and orchestration
│       └── transport/ # Communication abstractions
├── src/ # Implementations
│   └── (mirrors include/)
├── docs/ # Documentation
│   └── ARCHITECTURE.md
├── CMakeLists.txt
└── .clang-format
```

---

## Current Implementation Status
- `core/`, `clock/`, and `queue/` contain minimal working scaffolding.
- `algorithm/`, `policy/`, `transport/`, and `runtime/` are placeholders; behavior is not implemented yet.

---

## Layered Architecture

### 1. Application Layer (`apps/`)
**Purpose:** Entry points and experiment setup.

Responsibilities:
- MPI initialization and finalization,
- creation of runtime and algorithm instances,
- starting the event loop,
- logging and termination.

This layer contains **no algorithmic logic**.

---

### 2. Runtime Layer (`runtime/`)
**Purpose:** Orchestration and control flow.

Responsibilities:
- main event loop,
- dispatching incoming messages to the algorithm,
- triggering algorithm actions (request join, attempt group formation),
- coordinating transport and algorithm components.

The runtime layer does **not** implement synchronization logic.

---

### 3. Algorithm Layer (`algorithm/`)
**Purpose:** Distributed synchronization logic.

Responsibilities:
- handling incoming protocol messages,
- maintaining algorithm-specific state,
- deciding *when* an action may occur.

Planned variants:
- `lamport_queue_algorithm` – Lamport clock + total ordering + temporary initiator (placeholder).

This layer:
- is independent of MPI,
- depends only on abstract transport and clock interfaces,
- can be replaced by gossip- or DAG-based algorithms.

---

### 4. Clock Layer (`clock/`)
**Purpose:** Logical time and ordering.

Responsibilities:
- maintaining logical clocks,
- updating clocks on local and remote events,
- providing timestamps for ordering.

Implemented clocks:
- Lamport logical clock.

Algorithms depend on clocks via abstraction, allowing alternative clock models.

---

### 5. Queue Layer (`queue/`)
**Purpose:** Ordered storage of synchronization requests.

Responsibilities:
- maintaining locally replicated request order,
- insertion and removal of requests,
- exposing ordered views for decision making.

The queue:
- does not make decisions,
- does not know domain rules,
- only enforces ordering guarantees.

---

### 6. Policy Layer (`policy/`)
**Purpose:** Domain-specific group formation rules.

Responsibilities:
- selecting valid participant groups,
- enforcing capacity and composition constraints,
- resolving preferences (e.g. Hare/Bear weighting).

Policies are **fully decoupled** from synchronization algorithms.

---

### 7. Transport Layer (`transport/`)
**Purpose:** Communication abstraction.

Responsibilities:
- sending and receiving messages,
- message serialization and dispatch,
- mapping logical messages to concrete transport (MPI).

Planned transport:
- `mpi_transport` using MPI (placeholder).

Algorithms do not depend directly on MPI.

---

### 8. Core Types (`core/`)
**Purpose:** Shared data structures and definitions.

Contains:
- message definitions,
- ticket and identifier types,
- configuration and shared constants,
- common type aliases.

This layer is intentionally lightweight and dependency-free.

---

## Message Flow Overview

1. Application triggers a local participation request.
2. Algorithm generates protocol messages (`REQUEST`, `REPLY`).
3. Transport delivers messages.
4. Runtime dispatches messages to algorithm.
5. Algorithm updates clock and queue state.
6. Policy evaluates group feasibility.
7. Algorithm commits decision (`PARTY_START`).

All decisions are made **locally** based on replicated state.

---

## Design Principles

- **Separation of concerns:** synchronization, policy, and transport are independent.
- **Local decision making:** no central coordinator.
- **Replaceable algorithms:** algorithm layer can be swapped without affecting runtime.
- **Explicit abstractions:** clocks, transport, and policies are defined by interfaces.

---

## Extensibility

The architecture explicitly supports:
- leaderless algorithms (gossip-based dissemination),
- DAG-based causal ordering,
- alternative group selection policies,
- experimental metrics and tracing.

Only the algorithm and queue layers need to change for most extensions.

---

## Summary

This architecture prioritizes:
- correctness and clarity,
- experimental flexibility,
- clean separation between distributed systems theory and domain logic.

It enables both:
- compliance with course requirements,
- further research-oriented experimentation with distributed synchronization algorithms.
