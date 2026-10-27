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
│   ├── index.html                 # Responsive Single-Page Application (7 Tabs)
│   ├── css/
│   │   └── style.css              # Custom responsive glassmorphism styles
│   └── js/
│       ├── coderank_engine.js     # Client-side simulator of all ADS algorithms
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
│   ├── start_frontend.bat         # Windows web app launcher
│   ├── build.sh                   # Linux / macOS compilation script
│   ├── run_tests.sh               # Linux / macOS test runner
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
| **AVL Tree** | Member 2 | Self-balancing problem catalog indexing | $\mathcal{O}(\log n)$ search / insert |
| **Trie** (Prefix Tree) | Member 2 | Fast prefix search & tag autocompletion | $\mathcal{O}(L)$ string length |
| **Knuth-Morris-Pratt (KMP)**| Member 2 | Substring search across problem descriptions| $\mathcal{O}(N + M)$ linear |
| **Binomial Heap** | Member 3 | Priority submission queue with $O(\log n)$ merge | $\mathcal{O}(\log n)$ insert / extract / merge |
| **Disjoint Set Union (DSU)**| Member 3 | Team formation, merge requests, cycle detection | $\mathcal{O}(\alpha(n))$ near constant |
| **Freeze & Reveal Engine** | Member 3 | ICPC-style scoreboard freezing & unfreezing | $\mathcal{O}(S \log T)$ reveal phase |

---

## ⚡ Quick Start: Building & Running

### Windows (Command Prompt / PowerShell)
```cmd
cd scripts
build.bat
run_tests.bat
```

To run the interactive unified platform CLI:
```cmd
cd ..
coderank.exe
```

To launch the web interface:
```cmd
cd scripts
start_frontend.bat
```
*(Or simply double click `frontend/index.html` in your web browser)*

### Linux / macOS
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

## 🌐 Web Frontend Features

Open `frontend/index.html` in any modern web browser to access:
1. **Interactive Dashboard**: Live contest metrics, submission volumes, and active teams.
2. **Problems Catalog**: Searchable via **Trie** autocomplete, full-text filtered via **KMP**, and sorted via **AVL Tree**.
3. **Judging Queue**: Live interactive **Binomial Heap** queue with priority-based submission processing.
4. **User Standings**: Powered by **Treap** rank calculations and **Top-K Heap**.
5. **Team Leaderboard**: Dynamic team affiliations via **Disjoint Set Union (DSU)**.
6. **ICPC Freeze & Reveal**: Interactive freeze toggle with step-by-step submission reveals and leaderboard animations.
7. **ADS Visualizer & Benchmarks**: Live interactive visualizations of Binomial Trees, Treap splits/merges, AVL balance factors, and empirical benchmark charts.
