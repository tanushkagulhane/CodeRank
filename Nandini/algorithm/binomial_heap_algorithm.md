# Binomial Heap Algorithm

> **Note:** This document outlines the design and theoretical principles of the Binomial Heap used for the contest submission queue and serves as an algorithmic reference for Member 3.

## Aim
To manage a prioritized submission queue that supports insert, extract-min, and merging two submission queues, all in $O(\log n)$ worst-case time.

## Input
A set of submissions, each containing:
- `id`: unique submission integer ID
- `userId`: submitter user ID
- `problemId`: target problem ID
- `time`: contest submission time in minutes
- `verdict`: current verdict status ("Pending", "Accepted", "Wrong Answer", etc.)

Priority rule:
- By default (earlier contest time first): `time` ascending; ties broken by smaller `problemId`, then smaller submission `id`.
- Alternative mode (by problem): `problemId` ascending, ties broken by `time`.

## Structure
A **Binomial Heap** is a collection of binomial trees $B_0, B_1, B_2, \dots$ satisfying:
1. **Binomial Tree Structure:** A binomial tree $B_k$ has order $k$, contains $2^k$ nodes, has height $k$, and its root has degree $k$ whose children are roots of $B_{k-1}, B_{k-2}, \dots, B_0$.
2. **Min-Heap Property:** The key of each node is less than or equal to the keys of its children.
3. **Degree Property:** There is at most one binomial tree of any degree $k$ (analogous to binary representation of $n$).

Nodes use the Left-Child Right-Sibling (LCRS) representation:
- `child`: pointer to leftmost child.
- `sibling`: pointer to right sibling.
- `parent`: pointer to parent node.
- `degree`: number of children.

## Algorithm

### LinkTrees(y, z)
Attaches tree $y$ as the leftmost child of tree $z$ (where $z$'s key $\le y$'s key):
1. Set `y.parent = z`.
2. Set `y.sibling = z.child`.
3. Set `z.child = y`.
4. Increment `z.degree = z.degree + 1`.

### MergeRootLists(h1, h2)
Merges two root lists sorted in strictly increasing order of degree into a single root list sorted in non-decreasing order of degree (identical to the merge step of MergeSort in $O(\log n)$).

### UnionWith(other)
1. Merge root lists using `MergeRootLists`.
2. Traverse the combined root list using three pointers `prev`, `curr`, `next`:
   - **Case 1:** `curr.degree != next.degree` or three consecutive trees have the same degree $\to$ advance pointers.
   - **Case 2:** `curr.degree == next.degree` and `curr` has higher priority $\to$ link `next` as leftmost child of `curr`.
   - **Case 3:** `curr.degree == next.degree` and `next` has higher priority $\to$ link `curr` as leftmost child of `next`.
3. Terminate when end of list is reached.

### Insert(submission)
1. Construct a temporary 1-node binomial heap $H'$ of order 0 containing `submission`.
2. Call `UnionWith(H')`. Runs in $O(\log n)$ time.

### PeekMin()
1. Scan the root list of trees (at most $\lfloor \log_2 n \rfloor + 1$ roots).
2. Return the data of the root with minimum key. Runs in $O(\log n)$ (or $O(1)$ if tracked).

### ExtractMin()
1. Scan root list to locate root $x$ with minimum key.
2. Remove $x$ from root list.
3. Reverse the order of $x$'s children linked list (degrees become increasing: $0, 1, \dots, k-1$).
4. Create a temporary heap $H'$ with this reversed list as its root list.
5. Call `UnionWith(H')`.
6. Return $x$'s payload and free node $x$. Runs in $O(\log n)$ time.

## Example

**Submissions (ID, Time):**
```text
S1 (time = 45m), S2 (time = 20m), S3 (time = 60m), S4 (time = 15m), S5 (time = 30m)
```

**Heap State (n = 5 = 101_2):**
- One tree of order 0 ($B_0$ with 1 node): S3 (60m)
- Zero trees of order 1 ($B_1$)
- One tree of order 2 ($B_2$ with 4 nodes): Root S4 (15m), children S2 (20m) and S1 (45m).

**Output:**
```text
peekMin()    = S4 (15m)
extractMin() = S4 (15m) -> Next earliest submission to judge!
extractMin() = S2 (20m)
extractMin() = S5 (30m)
```

## Complexity
- **Insert:** $O(\log n)$
- **Find-Min / Peek:** $O(\log n)$
- **Extract-Min:** $O(\log n)$
- **Merge / Union:** $O(\log n)$
- **Space Complexity:** $O(n)$

Where $n$ is the number of submissions in the queue.
