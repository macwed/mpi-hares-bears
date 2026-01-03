# Hares and Bears
**Distributed Synchronization Algorithm (Lamport Clocks, MPI)**

> Architecture overview: [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)  
> Polish version: [README_PL.md](README_PL.md)

## Project Overview
This project implements a fully distributed synchronization algorithm based on **Lamport clocks** and **MPI communication**, solving a **group-based access problem to limited resources** without a central coordinator.

Each process represents a single participant, either a **Hare** or a **Bear**. Decisions about forming groups are made **locally and asynchronously**, based on each process’s local replica of a globally ordered request queue.

The project extends the classical distributed mutual exclusion problem with **group constraints and weighted participants**.

---

## System Model
- peer-to-peer processes,
- asynchronous distributed environment,
- reliable FIFO message delivery,
- no process failures,
- no shared memory.

---

## Synchronization Mechanism
- each process maintains a **local Lamport clock**,
- requests are globally ordered using *(timestamp, pid)*,
- each process stores a local copy of the request queue,
- no central arbiter or coordinator is used.

---

## Algorithm Outline
1. A process requests participation by sending a `REQUEST`.
2. The request is broadcast to all other processes.
3. After receiving `REPLY` messages from all processes, the request is enqueued.
4. The process at the head of the queue checks whether a valid group can be formed.
5. If conditions are satisfied, it initiates the event by sending `PARTY_START`.
6. Participants are removed from all local queues.

---

## Properties
- total ordering of requests (Lamport clocks),
- conflict-free access to shared resources,
- fully distributed decision making,
- deterministic resolution of concurrent requests.

---

## Known Limitations / Simplifications

### 1. No Process Failure Handling
The algorithm assumes:
- processes do not crash,
- all messages are eventually delivered.

Not implemented:
- failure detection,
- retries or recovery,
- reconfiguration after process loss.

---

### 2. Reliable FIFO Communication Assumption
MPI communication is assumed to be:
- reliable,
- FIFO-ordered between process pairs.

Message loss, duplication, or reordering are not considered.

---

### 3. Static Set of Processes
Each process:
- knows the total number of participants,
- waits for replies from all of them.

Dynamic joining or leaving of processes is not supported.

---

### 4. Deterministic Group Formation Rules
Group formation rules are simplified:
- total weight must be exactly equal to `S`,
- at least one Hare must be present,
- Bears are preferred when multiple configurations are possible.

No analysis of:
- global optimality,
- long-term fairness,
- starvation-freedom guarantees.

---

### 5. No Formal Complexity Analysis
The project focuses on:
- logical correctness,
- adherence to the Lamport model,
- practical MPI implementation.

Formal proofs of complexity or liveness properties are not included.

---

## Technologies
- **C++**
- **MPI** (e.g. OpenMPI)
- **CMake**

---

## Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/apps/mhb
mpirun -n <N> ./build/apps/mhb
```
