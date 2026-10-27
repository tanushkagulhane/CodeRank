# AVL Tree Algorithm Documentation

## Overview
An **AVL Tree** (named after inventors Adelson-Velsky and Landis) is a self-balancing Binary Search Tree (BST) where the heights of the two child subtrees of any node differ by at most one. If at any time they differ by more than one, rebalancing is done via tree rotations.

In CodeRank, the AVL Tree is utilized within the **Problem Management Service** to index competitive programming problems by their unique ID or numerical attributes (e.g. difficulty rating), guaranteeing logarithmic time operations for insertion, deletion, searching, and range queries.

---

## Key Characteristics & Balance Factor

### Balance Factor Calculation:
$$\text{BalanceFactor}(N) = \text{Height}(N.\text{left}) - \text{Height}(N.\text{right})$$

- In a valid AVL tree: $\text{BalanceFactor}(N) \in \{-1, 0, +1\}$ for all nodes $N$.
- A node with $\text{BalanceFactor} > 1$ is **left-heavy**.
- A node with $\text{BalanceFactor} < -1$ is **right-heavy**.

---

## Balancing Rotations

When an insertion or deletion causes $|\text{BalanceFactor}(N)| > 1$, one of four rotations is applied:

1. **Left-Left (LL) Case**:
   - Condition: $\text{BF}(N) > 1$ and $\text{BF}(N.\text{left}) \ge 0$
   - Fix: Single **Right Rotation** around $N$.

2. **Right-Right (RR) Case**:
   - Condition: $\text{BF}(N) < -1$ and $\text{BF}(N.\text{right}) \le 0$
   - Fix: Single **Left Rotation** around $N$.

3. **Left-Right (LR) Case**:
   - Condition: $\text{BF}(N) > 1$ and $\text{BF}(N.\text{left}) < 0$
   - Fix: **Left Rotation** on $N.\text{left}$, followed by **Right Rotation** on $N$.

4. **Right-Left (RL) Case**:
   - Condition: $\text{BF}(N) < -1$ and $\text{BF}(N.\text{right}) > 0$
   - Fix: **Right Rotation** on $N.\text{right}$, followed by **Left Rotation** on $N$.

---

## Complexity Analysis

| Operation | Average Case | Worst Case | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Search** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(1)$ |
| **Insert** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ recursion stack |
| **Delete** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ recursion stack |
| **In-order Traversal** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |

---

## CodeRank Use Cases
1. **Fast Problem Lookup by ID**: Constant $\mathcal{O}(\log n)$ lookup regardless of the number of cataloged problems.
2. **Ordered Retrieval**: In-order traversal produces sorted problem listings sorted by ID or difficulty rating.
3. **Dynamic Updates**: Adding newly authored contest problems or retiring obsolete problems retains balance in $\mathcal{O}(\log n)$ time.
