# CodeRank

A competitive-programming contest platform written in C++17. CodeRank manages a live leaderboard, a problem set, a judging queue and teams, and every feature is built on a hand-written data structure rather than the standard library containers.

> **Status:** In development.

## Goals

- Implement each core feature on the data structure best suited to it.
- Keep every module independent, with a small, clean interface.
- Compare the custom structures against a plain sorted list to show the performance difference.

## Features

**Leaderboard**
- Add a user or update their score
- "What is my rank?" lookup
- Show the top k users
- Count users between two scores, and find the highest score in a range

**Problems and search**
- Add and remove problems
- List problems in a difficulty range
- Prefix autocomplete for usernames and problem names
- Plagiarism check on submitted code

**Judging and teams**
- Submission queue and "judge next submission"
- Create and merge teams, with team score as the sum of member scores
- Freeze mode that hides leaderboard updates in the last hour, and a reveal command that applies them

## Data structures and complexity

| Feature | Structure | Operation | Time complexity |
|---|---|---|---|
| Leaderboard rank | Treap (with subtree sizes) | insert, erase, update, rank | O(log n) expected |
| Score range queries | Segment tree | count in range, max in range | O(log S) |
| Top-k users | Binary min-heap | top k of n users | O(n log k) |
| Problem list | AVL tree | insert, delete, search | O(log n) |
| Problem list | AVL tree | list a difficulty range | O(log n + m) |
| Autocomplete | Trie | insert, search | O(L) |
| Autocomplete | Trie | list by prefix | O(L + output) |
| Plagiarism check | KMP | match pattern in text | O(n + m) |
| Submission queue | Binomial heap | insert, merge, extract-min | O(log n) |
| Teams | Disjoint set union | union by rank, find with path compression | O(α(n)) amortized |
| Freeze and reveal | Pending update queue | reveal f frozen updates | O(f log n) |

Symbols: `n` = number of items, `S` = maximum score, `m` = number of results, `L` = length of the string, `f` = number of frozen updates.

## Shared classes

All modules use the same three classes:

```cpp
struct User       { int id; std::string name; int score; int team; };
struct Problem    { int id; std::string name; int difficulty; };
struct Submission { int user; int problem; int time; std::string verdict; };
```

## Design principles

- Each module exposes a few clean methods (insert, delete, search, plus its own queries) so the main menu can call it without reading the module's code.
- Each module runs and is tested on its own before it is merged to `main`.
- Randomized tests compare every structure against a simple brute-force version.

## Build and run

Requires a C++17 compiler such as g++.

```bash
g++ -std=c++17 -O2 main.cpp -o coderank
./coderank
```

On Windows, run `.\coderank.exe`.

## Documentation

Step-by-step algorithm descriptions, with examples and complexity, are kept in the `algorithm/` folder.
