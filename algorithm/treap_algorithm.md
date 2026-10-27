# Treap Algorithm

> **Note:** This document outlines my understanding of the treap used for the leaderboard and serves as a reference for implementation and further development.

## Aim
To store users in sorted leaderboard order and support insert, delete, score update and rank lookup, all in O(log n) expected time.

## Input
A set of users, each with:
- `id`: unique integer
- `score`: integer

Ordering rule: higher score comes first; if scores are equal, the smaller `id` comes first. Rank 1 is the best user.

## Structure
Each node stores `score`, `id`, a random `priority`, `size` (number of nodes in its subtree), and left/right children.
- It is a **binary search tree** on the ordering rule above.
- It is a **max-heap** on `priority`, which keeps the tree balanced on average.

## Algorithm

### Split(t, key)
Divides tree `t` into `L` (nodes strictly before `key`) and `R` (the rest).
1. If `t` is empty, return two empty trees.
2. If `t` comes before `key`, split `t.right` by `key`; attach the left part as `t.right`, update `t.size`, return `t` as `L`.
3. Otherwise split `t.left` by `key`; attach the right part as `t.left`, update `t.size`, return `t` as `R`.

### Merge(a, b)
Joins two trees where every node of `a` comes before every node of `b`.
1. If one tree is empty, return the other.
2. If `a.priority > b.priority`, set `a.right = Merge(a.right, b)`, update `a.size`, return `a`.
3. Otherwise set `b.left = Merge(a, b.left)`, update `b.size`, return `b`.

### Insert(score, id)
1. Create a new node with a random priority.
2. Split the tree by the new key into `L` and `R`.
3. Result = Merge(Merge(L, new node), R).

### Erase(score, id)
1. Walk down from the root using the ordering rule.
2. When the node with the matching `score` and `id` is found, replace it with Merge(its left, its right).
3. Update `size` on every node of the path while returning.
4. If the node is not found, report failure.

### Update score
1. Erase the old key `(oldScore, id)`.
2. Insert the new key `(newScore, id)`.

### Rank(score, id)
1. Set `res = 0` and start at the root.
2. If the current node matches the key, return `res + size(left) + 1`.
3. If the key comes before the current node, go left.
4. Otherwise add `size(left) + 1` to `res` and go right.
5. If the walk ends without a match, return -1.

### Kth(k)
1. Start at the root. Let `ls = size(left)`.
2. If `k == ls + 1`, this node is the answer.
3. If `k <= ls`, go left.
4. Otherwise set `k = k - ls - 1` and go right.

## Example

**Input users (id, score):**
```text
(1, 50)  (2, 80)  (3, 50)
```

**Sorted order:**
```text
user 2 (80), user 1 (50), user 3 (50)
```

**Output:**
```text
rank(user 2) = 1
rank(user 1) = 2
rank(user 3) = 3    (tie with user 1, larger id comes later)
kth(2)       = user 1
```

## Complexity
- **Time Complexity:** O(log n) expected for insert, erase, update, rank and kth
- **Space Complexity:** O(n)

Where `n` is the number of users.
