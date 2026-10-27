# Judging and Teams Algorithm (Member 3 Facade)

> **Note:** This document outlines how the JudgingService facade combines the Binomial Heap, Disjoint Set Union, Freeze/Reveal Engine, and Team Leaderboard.

## Aim
To provide a unified service interface that orchestrates the submission queue, automated judging, dynamic team mergers with score summation, contest freeze mode, and team leaderboard calculations.

## Input
- Submissions with `userId`, `problemId`, `time`, `verdict`, and `code`.
- Users and team definitions.
- Commands: `submit`, `judgeNext`, `mergeQueues`, `createTeam`, `mergeTeams`, `freeze`, `reveal`.

## Structure
- `BinomialHeap`: Priority queue for pending submissions.
- `DisjointSetUnion`: Partition of users into teams with union by rank and path compression.
- `FreezeRevealEngine`: Withheld scoreboard updates and pending queue.
- `TeamLeaderboard`: Dynamic rank generator based on team scores.

## Algorithm

### Submit(submission)
1. Assign a sequential submission ID if not already present.
2. Insert `submission` into the Binomial Heap in $O(\log n)$ time.

### JudgeNext(overrideVerdict, points)
1. Extract minimum submission from the Binomial Heap in $O(\log n)$ time (earliest submission time processed first).
2. Determine verdict ("Accepted", "Wrong Answer", etc.).
3. If "Accepted", award points.
4. Update user score in user table.
5. Update DSU team score: `dsu.updateUserScore(userId, points)`.
6. Pass result to `FreezeRevealEngine`:
   - If contest is frozen, defer public score update and queue into `pendingQueue`.
   - If not frozen, apply directly to public board.
7. Record submission into history and return result summary.

### MergeQueues(otherService)
1. Merge `otherService.submissionQueue_` into `this.submissionQueue_` using Binomial Heap union in $O(\log n)$ time.

### MergeTeams(team1, team2)
1. Execute DSU union: `dsu.unionSets(team1, team2)` in $O(\alpha(n))$ time.
2. Combine member lists and set the new team score to the sum of both teams' scores.

### Freeze and Reveal
1. `freeze()`: activate freeze mode.
2. `revealNext()`: apply next pending update to public board and broadcast update.
3. `revealAll()`: reveal all withheld updates.

## Complexity
- **Submit:** $O(\log n)$
- **JudgeNext:** $O(\log n)$
- **MergeQueues:** $O(\log n)$
- **Team operations (find, union, score update):** $O(\alpha(n))$ amortized
- **Team Leaderboard calculation:** $O(T \log T)$ where $T$ is the number of teams
- **Reveal all updates:** $O(f \log T)$ where $f$ is the number of frozen updates

Where $n$ is queue size, $T$ is team count, and $f$ is frozen update count.
