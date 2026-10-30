# CodeRank: Unified Competitive Programming Platform

CodeRank is a competitive programming platform that integrates **Advanced Data Structures (ADS)** with modern web technologies to provide interactive contest management, dynamic leaderboards, problem searching, submission judging, team management and performance benchmarking.

The project combines a modular C++ backend with an interactive web frontend, demonstrating the practical applications of advanced data structures and algorithms in a unified platform.

## Table of Contents

- [Project Overview](#project-overview)
- [Project Architecture](#project-architecture)
- [Contributors](#contributors)
- [Advanced Data Structures and Complexity Analysis](#advanced-data-structures-and-complexity-analysis)
- [System Architecture](#system-architecture)
- [Getting Started](#getting-started)
- [Running the Web Frontend](#running-the-web-frontend)
- [Web Application Features](#web-application-features)
- [Testing and Validation](#testing-and-validation)
- [Algorithm Documentation](#algorithm-documentation)
- [Technology Stack](#technology-stack)

---

## Project Overview

CodeRank provides an interactive environment for exploring data structures and algorithms through competitive programming workflows.

### Key Objectives

- Implement advanced data structures in C++.
- Apply data structures to practical problems such as ranking, searching and priority-based scheduling.
- Provide an interactive web interface for exploring algorithm behaviour.
- Compare advanced data structures with simpler implementations through benchmarking.
- Maintain modular, testable and reusable code.

### Core Modules

| Module | Purpose |
|---|---|
| Ranking and Score Analytics | Dynamic rankings, score queries and Top-K retrieval |
| Problem Management and Search | Problem indexing, prefix search, autocomplete and pattern matching |
| Submission Judging and Team Management | Submission queues, team operations and scoreboard management |

---

## Project Architecture

```text
CodeRank/
├── backend/
│   ├── include/
│   │   ├── Models.h
│   │   ├── common.h
│   │   ├── treap.h
│   │   ├── segtree.h
│   │   ├── topk_heap.h
│   │   ├── ranking_engine.h
│   │   ├── AVLTree.h
│   │   ├── Trie.h
│   │   ├── KMP.h
│   │   ├── ProblemService.h
│   │   ├── binomial_heap.h
│   │   ├── dsu.h
│   │   ├── freeze_reveal.h
│   │   ├── team_leaderboard.h
│   │   ├── judging_service.h
│   │   ├── timing_comparison.h
│   │   └── coderank_platform.h
│   │
│   └── src/
│       ├── AVLTree.cpp
│       ├── Trie.cpp
│       ├── KMP.cpp
│       ├── ProblemService.cpp
│       ├── judging_service.cpp
│       ├── timing_comparison.cpp
│       ├── coderank_platform.cpp
│       └── main.cpp
│
├── database/
│   ├── schema.sql
│   ├── seed_data.sql
│   ├── database.json
│   ├── users.csv
│   ├── problems.csv
│   ├── teams.csv
│   └── submissions.csv
│
├── frontend/
│   ├── index.html
│   ├── css/
│   │   └── style.css
│   └── js/
│       ├── coderank_engine.js
│       ├── data.js
│       └── app.js
│
├── tests/
│   ├── test_ranking.cpp
│   ├── test_problems_search.cpp
│   ├── test_judging_teams.cpp
│   └── test_integrated_platform.cpp
│
├── scripts/
│   ├── build.bat
│   ├── run_tests.bat
│   ├── start_frontend.bat
│   ├── build.sh
│   ├── run_tests.sh
│   └── run_demo.cpp
│
├── algorithm/
│   ├── treap_algorithm.md
│   ├── segment_tree_algorithm.md
│   ├── topk_heap_algorithm.md
│   ├── ranking_engine_algorithm.md
│   ├── avl_tree_algorithm.md
│   ├── trie_algorithm.md
│   ├── kmp_algorithm.md
│   ├── binomial_heap_algorithm.md
│   ├── disjoint_set_union_algorithm.md
│   ├── freeze_reveal_algorithm.md
│   └── judging_teams_algorithm.md
│
└── README.md
```

### Directory Overview

| Directory | Description |
|---|---|
| `backend/` | C++ implementations of data structures, algorithms and platform services |
| `database/` | SQL schemas, seed data and structured datasets |
| `frontend/` | Interactive web interface built with HTML, CSS and JavaScript |
| `tests/` | Unit tests and integration tests |
| `scripts/` | Build, test and frontend-launch scripts |
| `algorithm/` | Algorithm explanations and complexity analysis |

---

## Contributors

| Contributor | Contribution |
|-------------|-------------|
| **Tanushri Jadhav** | Treap, Segment Tree, Top-K Min-Heap, Dynamic Ranking Engine, Score Analytics, Ranking Unit Tests |
| **Tanushka Gulhane** | AVL Tree, Trie, KMP Pattern Matching, Problem Management & Search, Input Validation, Search Unit Tests |
| **Nandini** | Binomial Heap, Disjoint Set Union, Freeze & Reveal Scoreboard, Judging Service, Team Management, Benchmarking |

**Shared responsibilities:** platform integration, shared data models and interfaces, integration testing and algorithm documentation.

---

## Advanced Data Structures and Complexity Analysis

The following table summarizes the principal data structures used in CodeRank, their applications and their expected time complexities.

| Data Structure / Algorithm | Contributor | Application | Time Complexity |
|---|---|---|---|
| Treap | Tanushri | Dynamic ranking and rank queries | Expected O(log n) |
| Segment Tree | Tanushri | Range queries and updates | O(log n) per operation |
| Top-K Min-Heap | Tanushri | Top-K competitor retrieval | O(log K) per heap operation |
| AVL Tree | Tanushka | Problem indexing and search | O(log n) search, insertion and deletion |
| Trie | Tanushka | Prefix search and autocomplete | O(L) lookup for a key of length L |
| KMP | Tanushka | String pattern matching | O(N + M) |
| Binomial Heap | Nandini | Priority queue operations and merging | O(log n) for insertion, extraction and merge |
| Disjoint Set Union | Nandini | Team grouping and merging | O(α(n)) amortized per operation with path compression and union by rank |
| Freeze and Reveal Engine | Nandini | Contest scoreboard management | Depends on implementation; O(S log T) for ordered reveal updates |

**Complexity notation**

- `n`: Number of elements.
- `K`: Number of top-ranked competitors.
- `L`: Length of the search key.
- `N`: Length of the input text.
- `M`: Length of the search pattern.
- `α(n)`: Inverse Ackermann function.
- `S`: Number of pending submissions.
- `T`: Number of teams.

*The stated complexities assume standard implementations and the specified operating conditions. Actual performance depends on implementation details and workload.*

---

## System Architecture

CodeRank consists of two complementary layers: a compiled C++ backend and an interactive browser-based frontend.

### C++ Backend

The backend implements the core data structures, algorithms and platform services.

Its responsibilities include:

- Dynamic ranking and score analytics.
- Problem indexing and search.
- Submission queue management and judging.
- Team management and scoreboard operations.
- Performance benchmarking.
- Unit testing and platform integration.

The backend includes an interactive command-line interface for demonstrating the platform's functionality.

### Web Frontend

The frontend provides a visual interface for interacting with the platform's features.

**Technologies:** HTML5, CSS3, vanilla JavaScript and SVG.

The frontend provides:

- Interactive leaderboards.
- Problem indexing and prefix-based search.
- Code similarity analysis.
- Submission queue demonstrations.
- Team management and score aggregation.
- Scoreboard freeze and reveal.
- In-browser algorithm benchmarking.
- Dynamic tree visualization.

The frontend includes JavaScript implementations of the core data structures, allowing users to explore algorithm behaviour directly in a browser.

---

## Getting Started

### Prerequisites

Ensure the following tools are available:

- A C++ compiler supporting the C++ standard required by the source code.
- Python 3 for the local frontend server.
- A modern web browser.
- Git for version control.

On Windows, use a supported compiler available through your configured development environment. On Linux or macOS, a compatible GCC or Clang installation can be used.

### Building and Running on Windows

Run the following commands from the repository root in Command Prompt or PowerShell.

**Step 1: Build the project and run the tests**

```cmd
cd scripts
build.bat
run_tests.bat
```

**Step 2: Launch the interactive CLI**

Return to the repository root:

```cmd
cd ..
coderank.exe
```

Ensure that the executable name and output location match the configuration in `build.bat`.

### Building and Running on Linux / macOS

**Step 1: Make the scripts executable**

```bash
cd scripts
chmod +x build.sh run_tests.sh
```

**Step 2: Build the project**

```bash
./build.sh
```

**Step 3: Run the test suites**

```bash
./run_tests.sh
```

**Step 4: Launch the interactive CLI**

Return to the repository root:

```bash
cd ..
./coderank.out
```

Ensure that the executable name and output location match the configuration in `build.sh`.

---

## Running the Web Frontend

The frontend can be launched using a local HTTP server or opened directly in a browser.

### Option 1: Local HTTP Server - Recommended

On Windows, run:

```cmd
cd scripts
start_frontend.bat
```

The script is intended to launch a local Python HTTP server at:

```text
http://localhost:8000
```

Open this address in your browser if it does not open automatically.

### Option 2: Open Directly in a Browser

Open the following file:

```text
frontend/index.html
```

The bundled `frontend/js/data.js` provides a local dataset for standalone demonstrations, subject to the browser's compatibility and the frontend's data-loading implementation.

**Note:** Use the local HTTP server if browser security restrictions prevent scripts or datasets from loading correctly.

---

## Web Application Features

CodeRank provides eight interactive sections.

### 1. Leaderboard

Uses a Treap for dynamic rankings, a Segment Tree for score range queries and a Min-Heap for Top-K retrieval.

**Features**

- Dynamic contestant rankings.
- Rank lookup queries.
- Score range analysis.
- Top-K competitor retrieval.

### 2. Problemset

Uses an AVL Tree for balanced problem indexing and a Trie for prefix-based search.

**Features**

- Problem indexing and lookup.
- Prefix search and autocomplete.
- Problem creation with input validation.
- Problem deletion and AVL rebalancing.

### 3. Plagiarism Checker

Uses the Knuth–Morris–Pratt algorithm to identify matching code fragments.

**Features**

- Multi-file code comparison.
- Substring pattern matching.
- Configurable matching-window length.

*Note: KMP detects exact string matches. It does not independently establish plagiarism or identify semantically equivalent code.*

### 4. Judging Queue

Uses a Binomial Heap to manage submissions according to the configured queue priority.

**Features**

- Priority-based submission processing.
- Queue inspection and priority handling.
- Submission verdict simulation.
- Score updates based on configured verdict rules.

### 5. Teams and DSU

Uses Disjoint Set Union with path compression and union by rank to manage team groupings.

**Features**

- Team grouping and merging.
- Individual-to-team score aggregation.
- Dynamic team standings.

### 6. Scoreboard Freeze and Reveal

Simulates ICPC-style scoreboard freezing during the final phase of a contest.

**Features**

- Temporary suppression of selected scoreboard updates.
- Pending submission indicators.
- Step-by-step result revelation.
- Batch reveal and ranking updates.

### 7. ADS Benchmarks

Compares selected advanced data structures against simpler baseline implementations.

**Features**

- Treap versus sorted-array operations.
- Binomial Heap versus linear-search-based priority handling.
- DSU versus naive chain traversal.
- Execution-time measurements.
- Comparative speedup calculations.

Benchmark results depend on dataset size, implementation details and the execution environment.

### 8. Tree Visualizer

Provides an SVG-based visualization of the problem-indexing AVL Tree.

**Features**

- Dynamic node and edge rendering.
- Problem names and difficulty values.
- Parent-child relationships.
- Node balance-factor visualization.

---

## Testing and Validation

CodeRank includes dedicated test suites for individual modules and an integrated platform test suite.

| Test File | Testing Scope | Contributor |
|---|---|---|
| `test_ranking.cpp` | Treap, Segment Tree and Top-K Heap | Tanushri |
| `test_problems_search.cpp` | AVL Tree, Trie and KMP | Tanushka |
| `test_judging_teams.cpp` | Binomial Heap, DSU and Freeze/Reveal | Nandini |
| `test_integrated_platform.cpp` | Cross-module integration | All team members |

### Running the Tests

**Windows**

```cmd
cd scripts
run_tests.bat
```

**Linux / macOS**

```bash
cd scripts
./run_tests.sh
```

The test suites are designed to validate individual data structure operations and interactions between platform modules.

Actual test coverage and passing results should be confirmed by running the test scripts.

---

## Algorithm Documentation

The `algorithm/` directory contains Markdown documentation for the principal algorithms implemented in CodeRank.

| Documentation File | Topic |
|---|---|
| `treap_algorithm.md` | Treap operations and dynamic ranking |
| `segment_tree_algorithm.md` | Segment Tree queries and updates |
| `topk_heap_algorithm.md` | Top-K retrieval using a Min-Heap |
| `ranking_engine_algorithm.md` | Dynamic ranking engine |
| `avl_tree_algorithm.md` | AVL Tree operations and balancing |
| `trie_algorithm.md` | Prefix search and autocomplete |
| `kmp_algorithm.md` | Knuth–Morris–Pratt pattern matching |
| `binomial_heap_algorithm.md` | Binomial Heap operations |
| `disjoint_set_union_algorithm.md` | DSU, union by rank and path compression |
| `freeze_reveal_algorithm.md` | Scoreboard freezing and result revelation |
| `judging_teams_algorithm.md` | Submission judging and team management |

The documentation supports understanding the algorithm design, implementation, complexity and application of each data structure.

---

## Technology Stack

| Category | Technologies |
|---|---|
| Backend | C++ |
| Frontend | HTML5, CSS3, JavaScript |
| Visualization | SVG |
| Database and Data Storage | SQL, JSON, CSV |
| Testing | C++ unit tests and integration tests |
| Build Automation | Windows batch scripts, Bash |
| Local Development | Python HTTP server |

---

## Conclusion

CodeRank demonstrates how Advanced Data Structures can be applied to the design of an interactive competitive programming platform.

By combining dynamic ranking, balanced search trees, string-processing algorithms, priority queues, disjoint-set operations and visual benchmarking, the project connects theoretical concepts with practical software development.

Its modular architecture enables individual components to be developed and tested independently while supporting integration into a unified platform.

**Project Focus:** Advanced Data Structures, algorithm analysis, modular software design, testing and interactive visualization.
