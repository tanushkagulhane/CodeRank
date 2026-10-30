# Segment Tree Algorithm

> **Note:** This document outlines my understanding of the score segment tree and serves as a reference for implementation and further development.

## Aim
To count how many users have a score in a given range, and to find the highest score present in a range.

## Input
- A maximum possible score `S` (default 100000).
- Operations: add or remove one user with a given score, query `[lo, hi]`.

## Structure
The tree is built over score values `0 .. S`. Each leaf represents one score value. Every node covers a range of scores and stores:
- `cnt`: number of users whose score lies in the node's range
- `mx`: the highest score in the range that has at least one user, or -1 if none

## Algorithm

### Point update(score, delta)
`delta` is +1 to add a user and -1 to remove one.
1. Start at the root and move down towards the leaf for `score`.
2. At the leaf, add `delta` to `cnt`. Set `mx = score` if `cnt > 0`, otherwise `mx = -1`.
3. On the way back up, set `cnt = cnt(left) + cnt(right)` and `mx = max(mx(left), mx(right))`.

### Count range(lo, hi)
1. Clamp `lo` and `hi` into `[0, S]`. If `lo > hi`, return 0.
2. If the node's range lies completely outside the query, return 0.
3. If it lies completely inside the query, return its `cnt`.
4. Otherwise return the sum of the answers from both children.

### Max in range(lo, hi)
1. Clamp `lo` and `hi` into `[0, S]`. If `lo > hi`, return -1.
2. If the node's range lies completely outside the query, return -1.
3. If it lies completely inside the query, return its `mx`.
4. Otherwise return the larger answer of both children.

### Score change of a user
1. Run update(oldScore, -1).
2. Run update(newScore, +1).

## Example

**Scores present:**
```text
10, 20, 20, 50
```

**Output:**
```text
countRange(15, 50) = 3     (20, 20, 50)
countRange(60, 90) = 0
maxInRange(0, 30)  = 20
maxInRange(60, 90) = -1
```

## Complexity
- **Time Complexity:** O(log S) per update and per query
- **Space Complexity:** O(S)

Where `S` is the maximum score.
