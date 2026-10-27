# Top-K Heap Algorithm

> **Note:** This document outlines my understanding of the top-k binary heap and serves as a reference for implementation and further development.

## Aim
To find the k best users from all users without sorting everyone.

## Input
- A collection of `(score, id)` pairs
- An integer `k`

"Worse" means a lower score, or the same score with a larger id.

## Structure
A binary **min-heap** of at most `k` entries stored in an array. The root is the worst user among the current best k. For index `i`, the parent is `(i - 1) / 2` and the children are `2i + 1` and `2i + 2`.

## Algorithm

### Offer(score, id)
1. If `k <= 0`, do nothing.
2. If the heap has fewer than `k` entries, append the entry and sift it up.
3. Otherwise, if the root is worse than the new entry, replace the root with the new entry and sift it down.
4. Otherwise ignore the new entry.

### Sift up(i)
1. While `i > 0`, compare entry `i` with its parent.
2. If entry `i` is worse than its parent, swap them and move to the parent.
3. Otherwise stop.

### Sift down(i)
1. Find the worst among entry `i` and its two children.
2. If entry `i` is already the worst, stop.
3. Otherwise swap with that child and continue from the child's position.

### Extract sorted
1. Repeatedly remove the root (the worst remaining entry) and append it to a list.
2. Move the last entry to the root and sift down after every removal.
3. Reverse the list so the best user comes first.

### Top-k of all users
1. Create an empty heap with capacity `k`.
2. Offer every user to the heap.
3. Return the extracted sorted list.

## Example

**Input (id: score), k = 2:**
```text
1: 50    2: 80    3: 70    4: 60
```

**Steps:**
```text
offer 50 -> heap [50]
offer 80 -> heap [50, 80]       root = 50
offer 70 -> 50 is worse, replace root -> heap [70, 80]
offer 60 -> root 70 is not worse than 60, ignored
```

**Output:**
```text
user 2 (80), user 3 (70)
```

## Complexity
- **Time Complexity:** O(n log k)
- **Space Complexity:** O(k)

Where `n` is the number of users and `k` is the number of results requested.
