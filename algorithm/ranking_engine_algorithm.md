# Ranking Engine Algorithm

> **Note:** This document outlines how the leaderboard facade combines the treap, segment tree and top-k heap, and serves as a reference for the main menu integration.

## Aim
To give the menu one class that handles all leaderboard features and keeps the three data structures consistent.

## Input
- Users with `id`, `name`, `score` and `team`
- Commands: add or update, remove, rank, top k, count in range, max in range

## Structure
- Hash map `id -> User` for fast lookup of the current score
- Treap for rank queries
- Segment tree for score-range queries
- Top-k heap built on demand for top-k queries

## Algorithm

### Add or update(user)
1. Reject the score if it is outside `[0, maxScore]`.
2. If the id is new, store the user, insert `(score, id)` into the treap and add +1 at `score` in the segment tree.
3. If the id exists, read the old score from the hash map.
4. Update the treap from the old key to the new key.
5. Add -1 at the old score and +1 at the new score in the segment tree.
6. Save the new score and name in the hash map.

### Remove(id)
1. If the id is not in the hash map, report failure.
2. Erase `(score, id)` from the treap.
3. Add -1 at `score` in the segment tree.
4. Remove the id from the hash map.

### Rank(id)
1. Look up the user's score in the hash map. If missing, return -1.
2. Return the treap rank of `(score, id)`.

### Top k
1. Offer every user to a top-k heap of size `k`.
2. Extract the sorted list and map each id back to its user.

### Count / max in score range
1. Pass `[lo, hi]` to the segment tree and return its answer.

## Example

**Commands:**
```text
add (1, 50)   add (2, 80)   add (3, 50)   update (1 -> 90)
```

**Output:**
```text
rank(1)             = 1
rank(2)             = 2
topK(2)             = user 1 (90), user 2 (80)
countInRange(50,80) = 2
```

## Complexity
- **Add / update / remove / rank:** O(log n)
- **Top k:** O(n log k)
- **Count / max in range:** O(log S)
- **Space Complexity:** O(n + S)

Where `n` is the number of users and `S` is the maximum score.
