# Trie (Prefix Tree) Algorithm Documentation

## Overview
A **Trie** (derived from re**trie**val) is a specialized tree data structure used to store an associative array where the keys are usually strings. Unlike a standard binary search tree, no node in the tree stores the complete key associated with that node; instead, its position in the tree defines which key it is associated with.

In CodeRank, the Trie is used for **instant autocomplete, prefix-based problem searching, and tag filtering** (e.g. searching "DP", "Graph", "Greedy", or problem titles like "Two Sum").

---

## Structure & Architecture

Each node in the CodeRank Trie represents a single character and maintains:
1. `children`: A mapping or array of child nodes (e.g., character index 0-25 for lowercase letters, or 0-127 for ASCII).
2. `isEndOfWord`: A boolean flag indicating whether this node terminates a valid key.
3. `problemIds`: A list of problem IDs that match or start with this key, allowing fast $O(L)$ candidate lookups.

```
       (root)
       /    \
     'd'    't'
     /        \
   'p'        'w'
   [DP]         \
                'o'
               [Two Sum]
```

---

## Operations & Complexity

Let $L$ be the length of the string / query prefix, and $\Sigma$ be the alphabet size.

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Insert Key** | $\mathcal{O}(L)$ | $\mathcal{O}(L \cdot \Sigma)$ worst-case |
| **Search Exact Key** | $\mathcal{O}(L)$ | $\mathcal{O}(1)$ |
| **Prefix Search / Autocomplete** | $\mathcal{O}(L + K)$ | $\mathcal{O}(K)$ where $K$ is total characters in matched keys |
| **Delete Key** | $\mathcal{O}(L)$ | $\mathcal{O}(1)$ |

---

## Advantages in CodeRank
1. **Predictable Query Latency**: Lookup time depends solely on the search term length $L$ (typically $< 30$ chars), independent of the size of the problem bank ($N$).
2. **Instant As-You-Type Suggestions**: Traverses to the prefix node in $\mathcal{O}(L)$ time and immediately collects all matching problem titles and tags via DFS/BFS.
3. **No Hash Collisions**: Avoids hash function overhead and collision degradation seen in large hash maps.
