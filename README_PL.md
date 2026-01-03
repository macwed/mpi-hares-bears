## Kontekst akademicki
**Projekt został zrealizowany w ramach przedmiotu Przetwarzanie Rozproszone.**

# Zające i Niedźwiedzie
> Architektura: [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)  
> English version: [README.md](README.md)

## Cel projektu
Projekt demonstruje rozproszony algorytm synchronizacji procesów oparty o **zegary Lamporta i komunikację MPI**, rozwiązujący **problem grupowego dostępu do ograniczonych zasobów** bez centralnego koordynatora.
Każdy proces reprezentuje jednego uczestnika (**Zając** lub **Niedźwiedź**). Decyzje o formowaniu grup podejmowane są w sposób w pełni **asynchroniczny i rozproszony**, na podstawie lokalnych kopii globalnej kolejki żądań.

---

## Model systemu
- procesy równorzędne,
- środowisko asynchroniczne,
- niezawodna komunikacja FIFO,
- brak awarii procesów,
- brak współdzielonej pamięci.

---

## Mechanizm synchronizacji
- każdy proces utrzymuje lokalny zegar Lamporta,
- żądania są porządkowane globalnie przez parę (timestamp, pid),
- każdy proces przechowuje lokalną kopię kolejki żądań,
- brak centralnego arbitra.

---

## Algorytm (zarys)
1. Proces zgłasza chęć udziału w imprezie (REQUEST).
2. Żądanie jest rozgłaszane do wszystkich procesów.
3. Po otrzymaniu REPLY od wszystkich, proces trafia do kolejki.
4. Proces na początku kolejki sprawdza możliwość utworzenia grupy.
5. Jeśli warunki są spełnione, inicjuje imprezę (PARTY_START).
6. Uczestnicy imprezy są usuwani z kolejek wszystkich procesów.

---

## Własności
- totalny porządek żądań (Lamport),
- brak konfliktów dostępu do zasobów,
- pełna dystrybucja decyzji,
- deterministyczna kolejność rozpatrywania żądań.

## Ograniczenia i uproszczenia

### 1. Brak obsługi awarii procesów
Algorytm zakłada, że:
- procesy nie ulegają awarii,
- wszystkie wysłane komunikaty zostaną dostarczone.

Nie zaimplementowano:
- detekcji awarii,
- mechanizmów ponawiania,
- rekonfiguracji grupy po utracie procesu.

---

### 2. Założenie niezawodnej komunikacji FIFO
Komunikacja MPI traktowana jest jako:
- niezawodna,
- zachowująca kolejność wiadomości między parami procesów (FIFO).

---

### 3. Globalna wiedza o liczbie procesów
Każdy proces:
- zna liczbę pozostałych procesów, 
- oczekuje REPLY od wszystkich uczestników systemu.

Nie rozpatrywano:
- dynamicznego dołączania,
- dynamicznego opuszczania systemu.

---

### 4. Deterministyczne warunki grupowe
Warunki tworzenia grupy są uproszczone:
- suma wag dokładnie równa S,
- co najmniej jeden Zając,
- preferencja Niedźwiedzi rozstrzygana lokalnie.

Nie badano:
- optymalizacji globalnej,
- sprawiedliwości długoterminowej,
- strategii wyboru „najlepszej” grupy.

---

### 5. Brak formalnej analizy złożoności
Projekt koncentruje się na:
- poprawności logicznej,
- zgodności z modelem Lamporta,
- praktycznej implementacji w MPI.

Nie przeprowadzono:
- formalnej analizy złożoności czasowej,
- ani dowodu własności typu starvation-freedom.

---

## Technologie
- **C++**
- **MPI** (np. OpenMPI)
- **CMake**

---

## Build & Run

```bash
cmake -S . -B build
cmake --build build
./build/apps/mhb
mpirun -n <N> ./build/apps/mhb
```
