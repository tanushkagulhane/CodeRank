# CodeRank - Member 3: Judging & Teams (Nandini)

This module handles competitive programming judging, submission queues, dynamic team formations, ICPC-style contest scoreboard freeze and reveals, and performance benchmarks against plain sorted lists.

---

## Assigned Responsibilities

| Tier | Feature | Data Structure | Operations | Time Complexity |
|---|---|---|---|---|
| **Hard** | Submission Queue | **Binomial Heap** | `insert`, `peekMin`, `extractMin`, `merge` | $O(\log n)$ |
| **Medium** | Team Management | **Disjoint Set Union (DSU)** | `makeSet`, `find` (path compression), `unionSets` (union by rank) | $O(\alpha(n))$ amortized |
| **Easy** | Freeze & Reveal | **Pending Update Queue** | `enableFreeze`, `processJudgedSubmission`, `revealNext`, `revealAll` | $O(1)$ process, $O(f \log T)$ reveal |
| **Easy** | Team Leaderboard | **Dynamic Aggregator** | Team score = Sum of member scores | $O(T \log T)$ |
| **Shared** | Benchmark Suite | **Timing Comparison** | Benchmark vs Plain Sorted List / Naive Array | Tables & Speedup factor |
| **Shared** | Demo Automation | **Demo Script** | Complete interactive & scripted demo | `demo.bat` / `demo.sh` |

*Symbols: $n$ = number of submissions, $T$ = number of teams, $f$ = number of frozen updates, $\alpha(n)$ = Inverse Ackermann function.*

---

## Core Components

### 1. Binomial Heap (`binomial_heap.h`)
- Hand-written priority queue built on Binomial Trees ($B_0, B_1, \dots, B_k$).
- Left-Child Right-Sibling (LCRS) tree representation.
- Merges two submission queues in $O(\log n)$ worst-case time (similar to binary addition with carries).
- Configurable priority modes:
  - `BY_CONTEST_TIME` (default): Earlier submission time extracted first.
  - `BY_PROBLEM`: Lower problem ID extracted first.

### 2. Disjoint Set Union (`dsu.h`)
- Partitions users into teams.
- **Path Compression:** Flattens tree on `find(u)` in amortized $O(\alpha(n))$.
- **Union by Rank:** Keeps tree depth minimal by attaching smaller rank tree under larger rank tree.
- **Team Score Summation:** Maintains team score dynamically as the exact sum of all member scores. Updating an individual member's score dynamically updates the team score in $O(\alpha(n))$!

### 3. Freeze & Reveal Logic (`freeze_reveal.h`)
- **Contest Freeze:** In the final hour of the contest (or upon toggle), public leaderboard updates are withheld.
- Submissions are still evaluated by the judge, but updates are placed into a FIFO pending queue.
- **Dramatic Reveal:** When contest ends, withheld updates are applied sequentially (or in bulk) to reveal the final true rankings!

### 4. Team Leaderboard (`team_leaderboard.h`)
- Ranks teams descending by total score, breaking ties by team ID.
- Supports both **Public (Frozen)** standings and **Real (Internal)** standings.

### 5. JudgingService Facade (`judging_service.h` / `.cpp`)
- One clean facade unifying queue, judging, teams, and freeze/reveal logic.

### 6. Timing Comparison Benchmark (`timing_comparison.h` / `.cpp`)
- Measures execution time using `std::chrono::high_resolution_clock`.
- Compares:
  1. Binomial Heap insert ($O(N \log N)$) vs Plain Sorted Vector shift insert ($O(N^2)$).
  2. Binomial Heap merge ($O(\log N)$) vs Plain Sorted Vector merge ($O(N)$).
  3. Binomial Heap extract-min ($O(N \log N)$) vs Unsorted Vector linear scan ($O(N^2)$).
  4. DSU union by rank ($O(N \alpha(N))$) vs Naive Array re-labeling ($O(N^2)$).
  5. DSU find with path compression ($O(Q \alpha(N))$) vs Uncompressed tree traversal ($O(Q \cdot D)$).

---

## Build and Run

### 1. Run Unit & Integration Tests (5 Sections, 5+ test cases per method)
```bash
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp test_judging_teams.cpp -o test_judging_teams.exe
.\test_judging_teams.exe
```

### 2. Run Automated End-to-End Demo with Benchmarking
```bash
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp run_demo.cpp -o run_demo.exe
.\run_demo.exe
```

### 3. Run Interactive Menu
```bash
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp test_judging_teams.cpp main.cpp -o main.exe
.\main.exe
```

### 4. Windows One-Click Demo
```bash
.\demo.bat
```

---

## Algorithm Documentation
Detailed step-by-step algorithms, examples, and complexity proofs are located in `Nandini/algorithm/`:
- [`binomial_heap_algorithm.md`](algorithm/binomial_heap_algorithm.md)
- [`disjoint_set_union_algorithm.md`](algorithm/disjoint_set_union_algorithm.md)
- [`freeze_reveal_algorithm.md`](algorithm/freeze_reveal_algorithm.md)
- [`judging_teams_algorithm.md`](algorithm/judging_teams_algorithm.md)
