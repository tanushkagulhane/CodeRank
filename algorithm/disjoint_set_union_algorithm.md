# Disjoint Set Union (DSU) Algorithm

> **Note:** This document outlines the design and theoretical principles of the Disjoint Set Union data structure used for team formation and score tracking in CodeRank.

## Aim
To manage dynamic team formations and team mergers, maintain team score as the sum of member scores, and determine team membership in near-constant amortized time.

## Input
- Users with unique integer IDs.
- Operations:
  - `makeSet(userId, teamName, score)`
  - `find(userId)`
  - `unionSets(userA, userB)`
  - `updateUserScore(userId, delta)`

## Structure
- `parent[u]`: pointer to the parent node of user $u$.
- `rank[u]`: upper bound on the height of the subtree rooted at $u$ (for union by rank).
- `teamScore[root]`: cumulative sum of scores of all members in the set rooted at `root`.
- `teamMembers[root]`: vector of all user IDs in this team.

## Algorithm

### MakeSet(u, name, score)
1. Initialize `parent[u] = u` (self-loop root).
2. Set `rank[u] = 0`.
3. Set `teamScore[u] = score`.
4. Set `teamMembers[u] = {u}`.

### Find(u) (with Path Compression)
Traverses upwards to the representative root of the set containing $u$, and flattens the tree on the way back so all traversed nodes point directly to the root:
```text
function Find(u):
    if parent[u] != u:
        parent[u] = Find(parent[u])  // Path compression
    return parent[u]
```

### UnionSets(u, v) (with Union by Rank)
Merges the team containing $u$ with the team containing $v$:
1. Let `rootU = Find(u)` and `rootV = Find(v)`.
2. If `rootU == rootV`, they are already teammates; return false.
3. Compare `rank[rootU]` and `rank[rootV]`:
   - If `rank[rootU] < rank[rootV]`, swap `rootU` and `rootV`.
   - Set `parent[rootV] = rootU`.
   - If `rank[rootU] == rank[rootV]`, increment `rank[rootU] = rank[rootU] + 1`.
4. **Aggregate Team Score:**
   `teamScore[rootU] = teamScore[rootU] + teamScore[rootV]`.
5. **Merge Member Lists:**
   Append `teamMembers[rootV]` to `teamMembers[rootU]`, and erase `teamMembers[rootV]`.
6. Return true.

### UpdateUserScore(u, delta)
1. Let `root = Find(u)`.
2. Update `teamScore[root] = teamScore[root] + delta`.

## Example

**Users (ID, Score):**
```text
User 1 (tourist, 3800), User 2 (Petr, 3200), User 3 (Benq, 3600)
```

**Operations:**
```text
makeSet(1, 3800)  -> Team 1 score = 3800
makeSet(2, 3200)  -> Team 2 score = 3200
unionSets(1, 2)   -> Team 1 score = 3800 + 3200 = 7000
updateScore(2, +100) -> Team 1 score = 7100
```

## Complexity
- **MakeSet:** $O(1)$
- **Find (Path Compression):** $O(\alpha(n))$ amortized
- **Union (Union by Rank):** $O(\alpha(n))$ amortized
- **UpdateUserScore:** $O(\alpha(n))$ amortized
- **Space Complexity:** $O(n)$

Where $\alpha(n)$ is the Inverse Ackermann function (effectively $\le 4$ for all practical input sizes).
