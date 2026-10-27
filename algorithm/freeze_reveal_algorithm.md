# Freeze and Reveal Algorithm

> **Note:** This document outlines the ICPC-style contest scoreboard freeze and dramatic reveal mechanism implemented for CodeRank.

## Aim
To withhold public scoreboard updates during the final hour of a programming contest (the freeze period) while maintaining real backend evaluation, and to reveal the true final standings sequentially or in bulk at contest conclusion.

## Input
- Contest threshold time $T_{\text{freeze}}$ (default: 240 minutes in a 300-minute contest).
- Stream of judged submissions $(u, p, \Delta s, t, v)$ where:
  - $u$: user ID
  - $p$: problem ID
  - $\Delta s$: score change
  - $t$: submission contest time in minutes
  - $v$: verdict

## Structure
- `publicScores[u]`: visible score shown to contestants and spectators.
- `realScores[u]`: true internal score maintained by the evaluation server.
- `pendingQueue`: FIFO queue of `PendingUpdate` structs containing details of submissions processed during the freeze.
- `pendingCount[u]`: count of withheld attempts for user $u$.

## Algorithm

### ProcessJudgedSubmission(u, p, delta, t, v)
1. Always update `realScores[u] = realScores[u] + delta`.
2. Check freeze status:
   - If `isFrozen == true` OR $t \ge T_{\text{freeze}}$:
     - Keep `publicScores[u]` unchanged at its pre-freeze value.
     - Increment `pendingCount[u] = pendingCount[u] + 1`.
     - Construct `PendingUpdate` record and append to `pendingQueue`.
     - Public scoreboard displays `? +1 pending`.
   - Else (normal live period):
     - Immediately update `publicScores[u] = publicScores[u] + delta`.

### RevealNext()
Simulates the live dramatic unfreezing event:
1. If `pendingQueue` is empty, return false.
2. Dequeue the front `PendingUpdate` record from `pendingQueue`.
3. Update visible score:
   `publicScores[record.userId] = publicScores[record.userId] + record.deltaScore`.
4. Decrement `pendingCount[record.userId] = pendingCount[record.userId] - 1`.
5. Broadcast unfreezing event with old score, new score, and rank change.
6. Return true.

### RevealAll()
Repeatedly calls `RevealNext()` until `pendingQueue` is empty, unveiling the final definitive leaderboard.

## Complexity
- **Submission processing:** $O(1)$
- **RevealNext:** $O(1)$ to apply update; $O(\log n)$ to recompute rank
- **RevealAll:** $O(f \log n)$ where $f$ is the number of frozen updates and $n$ is total users
- **Space Complexity:** $O(f)$ for the pending update queue
