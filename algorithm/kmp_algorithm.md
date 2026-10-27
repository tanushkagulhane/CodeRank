# Knuth-Morris-Pratt (KMP) Substring Search Algorithm

## Overview
The **Knuth-Morris-Pratt (KMP)** algorithm is an efficient string searching algorithm that searches for occurrences of a "pattern" string $P$ within a "text" string $T$ in linear time $\mathcal{O}(|T| + |P|)$.

Traditional naive string searching has a worst-case time complexity of $\mathcal{O}(|T| \cdot |P|)$ when backtracking occurs frequently. KMP avoids backtracking on the text string by precomputing a **Longest Prefix Suffix (LPS)** array (also known as the $\pi$ table or failure function).

In CodeRank, KMP is utilized for **full-text problem description searching, deep keyword matching, and code plagiarism/template detection**.

---

## The LPS (Failure Function) Array

Given pattern $P$ of length $M$:
- $\text{LPS}[i]$ stores the length of the longest proper prefix of $P[0 \dots i]$ that is also a suffix of $P[0 \dots i]$.

### LPS Construction Algorithm:
1. Initialize `len = 0` and `i = 1`, with `lps[0] = 0`.
2. While `i < M`:
   - If `P[i] == P[len]`: increment `len`, set `lps[i] = len`, and increment `i`.
   - If `P[i] != P[len]`:
     - If `len != 0`: set `len = lps[len - 1]`.
     - Else: set `lps[i] = 0` and increment `i`.

---

## Matching Algorithm

1. Scan text $T$ using index $i$, and pattern $P$ using index $j$.
2. When $T[i] == P[j]$, increment both $i$ and $j$.
3. When $j == M$, a full match is recorded at index $i - j$. Reset $j = \text{LPS}[j - 1]$.
4. When a mismatch occurs ($T[i] \neq P[j]$):
   - If $j > 0$: fall back to $j = \text{LPS}[j - 1]$ without decrementing $i$.
   - If $j == 0$: advance $i = i + 1$.

---

## Complexity Analysis

| Phase | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **LPS Precomputation** | $\mathcal{O}(M)$ | $\mathcal{O}(M)$ |
| **Search Traversal** | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ |
| **Overall** | $\mathcal{O}(N + M)$ | $\mathcal{O}(M)$ |

Where $N = |T|$ (length of text) and $M = |P|$ (length of pattern).

---

## Applications in CodeRank
1. **Description Search**: Fast querying through lengthy problem descriptions without performance stalls.
2. **Tag & Keyword Filtering**: Robust pattern matching even with ambiguous or repeated substrings.
3. **Plagiarism / Code Similarity Checks**: Rapid substring sequence identification across submitted source files.
