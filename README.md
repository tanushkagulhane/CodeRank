# CodeRank: Unified Competitive Programming Platform

CodeRank is an enterprise-grade competitive programming and contest judging platform built using **Advanced Data Structures (ADS)** and modern web technologies. This unified codebase (`backup-main`) consolidates the work of all three team members into a clean, modular architecture segregated by functionality.

---

## 📁 Project Architecture & Directory Segregation

```
. (Repository Root)
├── backend/                       # C++ Core Engine & Advanced Data Structures
│   ├── include/                   # Unified Header Files & ADS Implementations
│   │   ├── Models.h               # Shared entity models (User, Problem, Submission, Team)
│   │   ├── common.h               # Common constants and utilities
│   │   ├── treap.h                # [Member 1] Cartesian Tree for dynamic ranking
│   │   ├── segtree.h              # [Member 1] Segment Tree for range score updates
│   │   ├── topk_heap.h            # [Member 1] Min-Heap for real-time Top-K leaders
│   │   ├── ranking_engine.h       # [Member 1] Dynamic User Ranking Engine
│   │   ├── AVLTree.h              # [Member 2] Self-Balancing BST for problem indexing
│   │   ├── Trie.h                 # [Member 2] Prefix Tree for search & autocompletion
│   │   ├── KMP.h                  # [Member 2] Knuth-Morris-Pratt for description searching
│   │   ├── ProblemService.h       # [Member 2] Problem Management Service
│   │   ├── binomial_heap.h        # [Member 3] Binomial Heap for priority judging queue
│   │   ├── dsu.h                  # [Member 3] Disjoint Set Union with path compression
│   │   ├── freeze_reveal.h        # [Member 3] ICPC-style Scoreboard Freeze & Reveal
│   │   ├── team_leaderboard.h     # [Member 3] Dynamic Team Leaderboard
│   │   ├── judging_service.h      # [Member 3] Automated Judging & Submission Engine
│   │   ├── timing_comparison.h    # [Member 3] Empirical benchmark (Heap vs Plain List)
│   │   └── coderank_platform.h    # Master unified platform orchestrator
│   └── src/                       # C++ Implementation Files
│       ├── AVLTree.cpp            # AVL Tree implementation
│       ├── Trie.cpp               # Trie implementation
│       ├── KMP.cpp                # KMP pattern matching implementation
│       ├── ProblemService.cpp     # Problem service implementation
│       ├── judging_service.cpp    # Judging service implementation
│       ├── timing_comparison.cpp  # Timing comparison implementation
│       ├── coderank_platform.cpp  # Master unified platform coordinator
│       └── main.cpp               # Interactive CLI demo application
│
├── database/                      # Relational & Structured Data Store
│   ├── schema.sql                 # Complete relational DDL (Users, Problems, Teams, Submissions)
│   ├── seed_data.sql              # SQL INSERT seed data
│   ├── database.json              # Structured JSON store for fast prototyping
│   ├── users.csv                  # User credentials and initial ratings
│   ├── problems.csv               # Contest problem bank
│   ├── teams.csv                  # Team roster and member affiliations
│   └── submissions.csv            # Contest submission logs
│
├── frontend/                      # Modern Dark Glassmorphic Web Application
│   ├── index.html                 # Responsive Single-Page Application (8 Tabs)
│   ├── css/
│   │   └── style.css              # Custom responsive glassmorphism styles
│   └── js/
│       ├── coderank_engine.js     # Client-side JavaScript implementation of all ADS algorithms
│       ├── data.js                # Standalone database copy for direct file:// browser access
│       └── app.js                 # UI controller, event bindings, and toast notifications
│
├── tests/                         # Unit & Master Integration Test Suites
│   ├── test_ranking.cpp           # [Member 1] Tests Treap, SegTree, Top-K Heap
│   ├── test_problems_search.cpp   # [Member 2] Tests AVL Tree, Trie, KMP
│   ├── test_judging_teams.cpp     # [Member 3] Tests Binomial Heap, DSU, Freeze/Reveal
│   └── test_integrated_platform.cpp # Master Integration Test Suite (100% assertions)
│
├── scripts/                       # Cross-Platform Build & Execution Automations
│   ├── build.bat                  # Windows compilation script
│   ├── run_tests.bat              # Windows test suite runner
│   ├── start_frontend.bat         # Windows web app launcher (Python HTTP server)
│   ├── build.sh                   # Linux / macOS compilation script (LF line endings)
│   ├── run_tests.sh               # Linux / macOS test runner (LF line endings)
│   └── run_demo.cpp               # Standalone interactive demo launcher
│
└── algorithm/                     # Comprehensive ADS Mathematical & Complexity Docs
    ├── treap_algorithm.md
    ├── segment_tree_algorithm.md
    ├── topk_heap_algorithm.md
    ├── ranking_engine_algorithm.md
    ├── avl_tree_algorithm.md
    ├── trie_algorithm.md
    ├── kmp_algorithm.md
    ├── binomial_heap_algorithm.md
    ├── disjoint_set_union_algorithm.md
    ├── freeze_reveal_algorithm.md
    └── judging_teams_algorithm.md
```

---

## 🧠 Advanced Data Structures Mapping

| ADS Data Structure | Contributor | Platform Role | Time Complexity |
| :--- | :--- | :--- | :--- |
| **Treap** (Cartesian Tree) | Member 1 | Dynamic rank-by-score & position queries | $\mathcal{O}(\log n)$ expected |
| **Segment Tree** | Member 1 | Range score aggregations & batch updates | $\mathcal{O}(\log n)$ query / update |
| **Top-K Min-Heap** | Member 1 | Tracking the real-time top $K$ competitors | $\mathcal{O}(\log K)$ insertion |
| **AVL Tree** | Member 2 | Self-balancing problem catalog indexing | $\mathcal{O}(\log n)$ search / insert / delete |
| **Trie** (Prefix Tree) | Member 2 | Fast prefix search & autocomplete | $\mathcal{O}(L)$ string length |
| **Knuth-Morris-Pratt (KMP)**| Member 2 | Code plagiarism substring matching | $\mathcal{O}(N + M)$ linear |
| **Binomial Heap** | Member 3 | Priority submission queue with $O(\log n)$ merge | $\mathcal{O}(\log n)$ insert / extract / merge |
| **Disjoint Set Union (DSU)**| Member 3 | Team formation, merge requests, root scoring | $\mathcal{O}(\alpha(n))$ near constant |
| **Freeze & Reveal Engine** | Member 3 | ICPC-style scoreboard freezing & unfreezing | $\mathcal{O}(S \log T)$ reveal phase |

---

## ⚡ Architecture & Execution Guide

CodeRank operates in two complementary layers:
- **C++ Backend Engine**: The core compiled engine implementing all ADS structures with strict memory management and unit tests. Executed via interactive CLI and compiled test suites.
- **Client-Side Web Frontend**: A pure vanilla JavaScript (HTML5/CSS3/ES6+, no frameworks, no npm, no build step) re-implementation of the exact same data structures, allowing instructors and students to interactively test ranking, tree rebalancing, judging queues, team unions, and live benchmarks in any browser.

### Running the C++ Backend & Tests

#### Windows (Command Prompt / PowerShell)
```cmd
cd scripts
build.bat
run_tests.bat
```
To run the interactive CLI application:
```cmd
cd ..
coderank.exe
```

#### Linux / macOS
```bash
cd scripts
chmod +x build.sh run_tests.sh
./build.sh
./run_tests.sh
```
To run the master binary:
```bash
cd ..
./coderank.out
```

---

### Running the Web Frontend

You can launch the web frontend in either of two ways:

1. **Local HTTP Server (Recommended)**:
   ```cmd
   cd scripts
   start_frontend.bat
   ```
   This automatically launches Python's HTTP server at `http://localhost:8000` and opens the browser.
2. **Direct File Open**:
   Simply open `frontend/index.html` directly in any web browser. The bundled `frontend/js/data.js` ensures full dataset access (16 users, 32 problems, 8 teams) without CORS restrictions.

---

## 🌐 Web Frontend Tabs & Features

The web frontend includes 8 interactive tabs:
1. 🏆 **Leaderboard**: Live contestant rankings dynamically managed by a **Treap** with $\mathcal{O}(\log n)$ rank lookups, score range distribution counting via **Segment Tree**, Top-K leader filtering via **Min-Heap**, and "What's My Rank?" query dialogs.
2. 📚 **Problemset**: Problems catalog balanced by difficulty via **AVL Tree**, real-time prefix search via **Trie**, dynamic "Add Problem" form with input validation, and per-card "Delete" button with 4-case AVL rotations.
3. 🛡️ **Plagiarism Checker**: Multi-file code similarity inspector utilizing the **KMP (Knuth-Morris-Pratt)** pattern matching algorithm to detect code plagiarism with $\ge 30$-character sliding windows.
4. ⚡ **Judging Queue**: Priority submission queue managed by a **Binomial Heap** (min-heap ordered by contest submission time), featuring peek telemetry and verdict-aware judging (Accepted: +100 pts, Wrong Answer / TLE: 0 pts).
5. 👥 **Teams & DSU**: Live team standings and dynamic team merges powered by **Disjoint Set Union (DSU)** with Union by Rank and Path Compression, accurately mapping individual member points to team totals.
6. ❄️ **Freeze & Reveal**: ICPC-style scoreboard freeze mode for the final contest hour with pending submission indicators on the leaderboard, followed by dramatic step-by-step ("Reveal Next") or batch ("Reveal All") rank climb unveilings.
7. 📊 **ADS Benchmarks**: In-browser live microsecond diagnostics comparing custom ADS against naive implementations (Treap vs Sorted Array, Binomial Heap vs Linear Search, DSU vs Naive Chain Walk) with live speedup factors and backend C++ reference data.
8. 🌳 **Visualizer**: Dynamic SVG recursive diagram of the problem **AVL Tree**, illustrating node difficulty, parent-child edges, problem names, and real-time balance factors (BF).
