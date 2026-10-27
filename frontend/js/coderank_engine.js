// coderank_engine.js - Pure JavaScript implementation of CodeRank Advanced Data Structures
// Directly mirrors the C++ algorithms for Treap, SegTree, TopKHeap, AVL, Trie, KMP, BinomialHeap, DSU

// =====================================================================
// MEMBER 1: RANKING ENGINE (Treap, Segment Tree, Binary Heap)
// =====================================================================

// Treap with Subtree Sizes for O(log n) Rank Queries
class TreapNode {
  constructor(score, id, name) {
    this.score = score;
    this.id = id;
    this.name = name;
    this.priority = Math.random();
    this.size = 1;
    this.left = null;
    this.right = null;
  }
}

class Treap {
  constructor() {
    this.root = null;
  }

  size(node = this.root) {
    return node ? node.size : 0;
  }

  updateSize(node) {
    if (node) {
      node.size = 1 + this.size(node.left) + this.size(node.right);
    }
  }

  before(score1, id1, score2, id2) {
    return score1 > score2 || (score1 === score2 && id1 < id2);
  }

  split(t, score, id) {
    if (!t) return [null, null];
    if (this.before(score, id, t.score, t.id)) {
      const [l, r] = this.split(t.left, score, id);
      t.left = r;
      this.updateSize(t);
      return [l, t];
    } else {
      const [l, r] = this.split(t.right, score, id);
      t.right = l;
      this.updateSize(t);
      return [t, r];
    }
  }

  merge(l, r) {
    if (!l || !r) return l || r;
    if (l.priority > r.priority) {
      l.right = this.merge(l.right, r);
      this.updateSize(l);
      return l;
    } else {
      r.left = this.merge(l, r.left);
      this.updateSize(r);
      return r;
    }
  }

  insert(score, id, name) {
    this.erase(score, id);
    const n = new TreapNode(score, id, name);
    const [l, r] = this.split(this.root, score, id);
    this.root = this.merge(this.merge(l, n), r);
  }

  erase(score, id) {
    let removed = false;
    const removeNode = (t) => {
      if (!t) return null;
      if (t.score === score && t.id === id) {
        removed = true;
        return this.merge(t.left, t.right);
      }
      if (this.before(score, id, t.score, t.id)) {
        t.left = removeNode(t.left);
      } else {
        t.right = removeNode(t.right);
      }
      this.updateSize(t);
      return t;
    };
    this.root = removeNode(this.root);
    return removed;
  }

  rank(score, id) {
    let res = 0;
    let t = this.root;
    while (t) {
      if (t.score === score && t.id === id) {
        return res + this.size(t.left) + 1;
      }
      if (this.before(score, id, t.score, t.id)) {
        t = t.left;
      } else {
        res += this.size(t.left) + 1;
        t = t.right;
      }
    }
    return -1;
  }

  toSortedArray(node = this.root, arr = []) {
    if (!node) return arr;
    this.toSortedArray(node.left, arr);
    arr.push({ id: node.id, name: node.name, score: node.score });
    this.toSortedArray(node.right, arr);
    return arr;
  }
}

// Segment Tree over Scores [0, maxScore]
class ScoreSegTree {
  constructor(maxScore = 10000) {
    this.n = maxScore + 1;
    this.cnt = new Array(4 * this.n).fill(0);
    this.mx = new Array(4 * this.n).fill(-1);
  }

  add(score, delta, node = 1, l = 0, r = this.n - 1) {
    if (l === r) {
      this.cnt[node] += delta;
      this.mx[node] = this.cnt[node] > 0 ? l : -1;
      return;
    }
    const m = Math.floor((l + r) / 2);
    if (score <= m) this.add(score, delta, 2 * node, l, m);
    else this.add(score, delta, 2 * node + 1, m + 1, r);
    this.cnt[node] = this.cnt[2 * node] + this.cnt[2 * node + 1];
    this.mx[node] = Math.max(this.mx[2 * node], this.mx[2 * node + 1]);
  }

  countRange(lo, hi, node = 1, l = 0, r = this.n - 1) {
    if (hi < l || r < lo) return 0;
    if (lo <= l && r <= hi) return this.cnt[node];
    const m = Math.floor((l + r) / 2);
    return this.countRange(lo, hi, 2 * node, l, m) + this.countRange(lo, hi, 2 * node + 1, m + 1, r);
  }

  maxInRange(lo, hi, node = 1, l = 0, r = this.n - 1) {
    if (hi < l || r < lo) return -1;
    if (lo <= l && r <= hi) return this.mx[node];
    const m = Math.floor((l + r) / 2);
    return Math.max(this.maxInRange(lo, hi, 2 * node, l, m), this.maxInRange(lo, hi, 2 * node + 1, m + 1, r));
  }
}

// =====================================================================
// MEMBER 2: PROBLEMS & SEARCH (AVL Tree, Trie, KMP)
// =====================================================================

// AVL Tree for Problem Catalog by Difficulty
class AVLNode {
  constructor(problem) {
    this.problem = problem;
    this.height = 1;
    this.left = null;
    this.right = null;
  }
}

class AVLTree {
  constructor() {
    this.root = null;
  }

  height(node) {
    return node ? node.height : 0;
  }

  balance(node) {
    return node ? this.height(node.left) - this.height(node.right) : 0;
  }

  rightRotate(y) {
    const x = y.left;
    const t2 = x.right;
    x.right = y;
    y.left = t2;
    y.height = Math.max(this.height(y.left), this.height(y.right)) + 1;
    x.height = Math.max(this.height(x.left), this.height(x.right)) + 1;
    return x;
  }

  leftRotate(x) {
    const y = x.right;
    const t2 = y.left;
    y.left = x;
    x.right = t2;
    x.height = Math.max(this.height(x.left), this.height(x.right)) + 1;
    y.height = Math.max(this.height(y.left), this.height(y.right)) + 1;
    return y;
  }

  insert(problem) {
    if (typeof problem === 'number') {
      problem = { id: problem, difficulty: problem, name: "Problem " + problem, category: "General" };
    }
    const add = (node, p) => {
      if (!node) return new AVLNode(p);
      if (p.difficulty < node.problem.difficulty || (p.difficulty === node.problem.difficulty && p.id < node.problem.id)) {
        node.left = add(node.left, p);
      } else {
        node.right = add(node.right, p);
      }
      node.height = Math.max(this.height(node.left), this.height(node.right)) + 1;
      const bal = this.balance(node);

      // LL
      if (bal > 1 && (p.difficulty < node.left.problem.difficulty || (p.difficulty === node.left.problem.difficulty && p.id < node.left.problem.id)))
        return this.rightRotate(node);
      // RR
      if (bal < -1 && (p.difficulty > node.right.problem.difficulty || (p.difficulty === node.right.problem.difficulty && p.id > node.right.problem.id)))
        return this.leftRotate(node);
      // LR
      if (bal > 1) {
        node.left = this.leftRotate(node.left);
        return this.rightRotate(node);
      }
      // RL
      if (bal < -1) {
        node.right = this.rightRotate(node.right);
        return this.leftRotate(node);
      }
      return node;
    };
    this.root = add(this.root, problem);
  }

  delete(problemId, difficulty) {
    if (typeof problemId === 'object' && problemId !== null) {
      difficulty = problemId.difficulty;
      problemId = problemId.id;
    }
    let targetId = problemId;
    let targetDiff = difficulty;
    if (targetDiff === undefined) {
      const found = this.listAll().find(p => p.id === targetId || p.difficulty === targetId);
      if (found) {
        targetId = found.id;
        targetDiff = found.difficulty;
      } else {
        return false;
      }
    }

    const minValueNode = (node) => {
      let current = node;
      while (current.left) current = current.left;
      return current;
    };

    const remove = (node, id, diff) => {
      if (!node) return null;

      if (diff < node.problem.difficulty || (diff === node.problem.difficulty && id < node.problem.id)) {
        node.left = remove(node.left, id, diff);
      } else if (diff > node.problem.difficulty || (diff === node.problem.difficulty && id > node.problem.id)) {
        node.right = remove(node.right, id, diff);
      } else {
        // Node found: handle 0, 1, or 2 children
        if (!node.left || !node.right) {
          node = node.left || node.right;
        } else {
          // 2 children: get inorder successor (min node in right subtree)
          const successor = minValueNode(node.right);
          node.problem = successor.problem;
          node.right = remove(node.right, successor.problem.id, successor.problem.difficulty);
        }
      }

      if (!node) return null;

      // Update height
      node.height = Math.max(this.height(node.left), this.height(node.right)) + 1;
      const bal = this.balance(node);

      // Rebalance with all 4 rotation cases
      // Left Left Case
      if (bal > 1 && this.balance(node.left) >= 0) {
        return this.rightRotate(node);
      }
      // Left Right Case
      if (bal > 1 && this.balance(node.left) < 0) {
        node.left = this.leftRotate(node.left);
        return this.rightRotate(node);
      }
      // Right Right Case
      if (bal < -1 && this.balance(node.right) <= 0) {
        return this.leftRotate(node);
      }
      // Right Left Case
      if (bal < -1 && this.balance(node.right) > 0) {
        node.right = this.rightRotate(node.right);
        return this.leftRotate(node);
      }

      return node;
    };

    this.root = remove(this.root, targetId, targetDiff);
    return true;
  }

  listInRange(minDiff, maxDiff) {
    const res = [];
    const traverse = (node) => {
      if (!node) return;
      if (node.problem.difficulty >= minDiff) traverse(node.left);
      if (node.problem.difficulty >= minDiff && node.problem.difficulty <= maxDiff) res.push(node.problem);
      if (node.problem.difficulty <= maxDiff) traverse(node.right);
    };
    traverse(this.root);
    return res;
  }

  listAll() {
    return this.listInRange(0, 99999);
  }
}

// Trie for Autocomplete
class TrieNode {
  constructor() {
    this.children = {};
    this.isEndOfWord = false;
    this.originalWord = "";
  }
}

class Trie {
  constructor() {
    this.root = new TrieNode();
  }

  insert(word) {
    let curr = this.root;
    const lower = word.toLowerCase();
    for (const ch of lower) {
      if (!curr.children[ch]) curr.children[ch] = new TrieNode();
      curr = curr.children[ch];
    }
    curr.isEndOfWord = true;
    curr.originalWord = word;
  }

  search(word) {
    let curr = this.root;
    for (const ch of word.toLowerCase()) {
      if (!curr.children[ch]) return false;
      curr = curr.children[ch];
    }
    return curr.isEndOfWord;
  }

  getAllWithPrefix(prefix, limit = 10) {
    let curr = this.root;
    for (const ch of prefix.toLowerCase()) {
      if (!curr.children[ch]) return [];
      curr = curr.children[ch];
    }
    const results = [];
    const dfs = (node) => {
      if (!node || results.length >= limit) return;
      if (node.isEndOfWord) results.push(node.originalWord);
      for (const key of Object.keys(node.children).sort()) {
        dfs(node.children[key]);
      }
    };
    dfs(curr);
    return results;
  }

  delete(word) {
    if (!word) return false;
    const lower = word.toLowerCase();
    const removeHelper = (node, depth) => {
      if (!node) return false;
      if (depth === lower.length) {
        if (!node.isEndOfWord) return false;
        node.isEndOfWord = false;
        node.originalWord = "";
        return Object.keys(node.children).length === 0;
      }
      const ch = lower[depth];
      if (!node.children[ch]) return false;
      const shouldDeleteChild = removeHelper(node.children[ch], depth + 1);
      if (shouldDeleteChild) {
        delete node.children[ch];
        return !node.isEndOfWord && Object.keys(node.children).length === 0;
      }
      return false;
    };
    return removeHelper(this.root, 0);
  }

  remove(word) {
    return this.delete(word);
  }
}

// KMP Plagiarism Checker
class KMP {
  static buildLPS(pattern) {
    const lps = new Array(pattern.length).fill(0);
    let len = 0, i = 1;
    while (i < pattern.length) {
      if (pattern[i] === pattern[len]) {
        len++;
        lps[i] = len;
        i++;
      } else {
        if (len !== 0) len = lps[len - 1];
        else { lps[i] = 0; i++; }
      }
    }
    return lps;
  }

  static search(text, pattern) {
    if (!pattern || !text) return [];
    const lps = this.buildLPS(pattern);
    const matches = [];
    let i = 0, j = 0;
    while (i < text.length) {
      if (pattern[j] === text[i]) { i++; j++; }
      if (j === pattern.length) {
        matches.push(i - j);
        j = lps[j - 1];
      } else if (i < text.length && pattern[j] !== text[i]) {
        if (j !== 0) j = lps[j - 1];
        else i++;
      }
    }
    return matches;
  }

  static normalizeCode(code) {
    // Remove comments, normalize whitespace
    return code
      .replace(/\/\/.*$/gm, '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\s+/g, ' ')
      .trim();
  }

  static checkPlagiarism(newCode, problemId, storedSubmissions, chunkLen = 30) {
    const cleanNew = this.normalizeCode(newCode);
    if (cleanNew.length < chunkLen) {
      return { isPlagiarized: false };
    }

    for (const stored of storedSubmissions) {
      if (stored.problemId !== problemId) continue;
      const cleanStored = this.normalizeCode(stored.code || "");
      if (cleanStored.length < chunkLen) continue;

      // Scan chunks of new code across stored code using KMP
      for (let start = 0; start <= cleanNew.length - chunkLen; start += Math.floor(chunkLen / 2)) {
        const chunk = cleanNew.substr(start, chunkLen);
        const matches = this.search(cleanStored, chunk);
        if (matches.length > 0) {
          return {
            isPlagiarized: true,
            matchedSubmission: stored,
            matchedChunk: chunk
          };
        }
      }
    }
    return { isPlagiarized: false };
  }
}

// =====================================================================
// MEMBER 3: JUDGING & TEAMS (Binomial Heap, DSU, Freeze/Reveal)
// =====================================================================

// Binomial Heap Node
class BinomialNode {
  constructor(data) {
    this.data = data;
    this.degree = 0;
    this.parent = null;
    this.child = null;
    this.sibling = null;
  }
}

class BinomialHeap {
  constructor(priorityMode = 'time') {
    this.head = null;
    this.count = 0;
    this.priorityMode = priorityMode; // 'time' or 'problem'
  }

  hasHigherPriority(a, b) {
    if (this.priorityMode === 'time') {
      if (a.time !== b.time) return a.time < b.time;
      if (a.problemId !== b.problemId) return a.problemId < b.problemId;
      return a.id < b.id;
    } else {
      if (a.problemId !== b.problemId) return a.problemId < b.problemId;
      if (a.time !== b.time) return a.time < b.time;
      return a.id < b.id;
    }
  }

  linkTrees(child, parent) {
    child.parent = parent;
    child.sibling = parent.child;
    parent.child = child;
    parent.degree++;
  }

  mergeRootLists(h1, h2) {
    if (!h1) return h2;
    if (!h2) return h1;
    let head = null, tail = null;
    const append = (n) => {
      if (!head) head = tail = n;
      else { tail.sibling = n; tail = n; }
    };

    while (h1 && h2) {
      if (h1.degree <= h2.degree) {
        const next = h1.sibling;
        append(h1);
        h1 = next;
      } else {
        const next = h2.sibling;
        append(h2);
        h2 = next;
      }
    }
    if (h1) tail.sibling = h1;
    if (h2) tail.sibling = h2;
    return head;
  }

  unionWith(otherHeap) {
    if (!otherHeap.head) return;
    if (!this.head) {
      this.head = otherHeap.head;
      this.count = otherHeap.count;
      otherHeap.head = null;
      otherHeap.count = 0;
      return;
    }

    let newHead = this.mergeRootLists(this.head, otherHeap.head);
    this.head = null;
    otherHeap.head = null;

    if (!newHead) return;

    let prev = null, curr = newHead, next = curr.sibling;
    while (next) {
      if (curr.degree !== next.degree || (next.sibling && next.sibling.degree === curr.degree)) {
        prev = curr;
        curr = next;
      } else if (this.hasHigherPriority(curr.data, next.data)) {
        curr.sibling = next.sibling;
        this.linkTrees(next, curr);
      } else {
        if (!prev) newHead = next;
        else prev.sibling = next;
        this.linkTrees(curr, next);
        curr = next;
      }
      next = curr.sibling;
    }

    this.head = newHead;
    this.count += otherHeap.count;
    otherHeap.count = 0;
  }

  insert(sub) {
    const temp = new BinomialHeap(this.priorityMode);
    temp.head = new BinomialNode(sub);
    temp.count = 1;
    this.unionWith(temp);
  }

  peekMin() {
    if (!this.head) return null;
    let minNode = this.head, curr = this.head.sibling;
    while (curr) {
      if (this.hasHigherPriority(curr.data, minNode.data)) minNode = curr;
      curr = curr.sibling;
    }
    return minNode.data;
  }

  extractMin() {
    if (!this.head) return null;
    let minNode = this.head, minPrev = null, curr = this.head.sibling, prev = this.head;
    while (curr) {
      if (this.hasHigherPriority(curr.data, minNode.data)) {
        minNode = curr;
        minPrev = prev;
      }
      prev = curr;
      curr = curr.sibling;
    }

    if (!minPrev) this.head = minNode.sibling;
    else minPrev.sibling = minNode.sibling;

    // Reverse minNode children
    let childHead = null, childCurr = minNode.child;
    while (childCurr) {
      const next = childCurr.sibling;
      childCurr.sibling = childHead;
      childCurr.parent = null;
      childHead = childCurr;
      childCurr = next;
    }

    const temp = new BinomialHeap(this.priorityMode);
    temp.head = childHead;
    const childCnt = (1 << minNode.degree) - 1;
    temp.count = childCnt;
    this.count -= (childCnt + 1);

    this.unionWith(temp);
    return minNode.data;
  }

  toVector() {
    const list = [];
    const traverse = (node) => {
      if (!node) return;
      list.push(node.data);
      traverse(node.child);
      traverse(node.sibling);
    };
    traverse(this.head);
    return list;
  }
}

// Disjoint Set Union (DSU) for Teams
class DisjointSetUnion {
  constructor() {
    this.parent = {};
    this.rank = {};
    this.teamScore = {};
    this.userScore = {};
    this.teamName = {};
    this.teamMembers = {};
  }

  makeSet(userId, name = "", score = 0) {
    if (this.parent[userId] !== undefined) return;
    this.parent[userId] = userId;
    this.rank[userId] = 0;
    this.teamScore[userId] = score;
    this.userScore[userId] = score;
    this.teamName[userId] = name || `Team_${userId}`;
    this.teamMembers[userId] = [userId];
  }

  find(u) {
    if (this.parent[u] === undefined) return -1;
    if (this.parent[u] !== u) {
      this.parent[u] = this.find(this.parent[u]); // Path compression
    }
    return this.parent[u];
  }

  unionSets(u, v) {
    let rootU = this.find(u);
    let rootV = this.find(v);
    if (rootU === -1 || rootV === -1 || rootU === rootV) return false;

    // Union by rank
    if (this.rank[rootU] < this.rank[rootV]) {
      [rootU, rootV] = [rootV, rootU];
    }

    this.parent[rootV] = rootU;
    if (this.rank[rootU] === this.rank[rootV]) {
      this.rank[rootU]++;
    }

    // Team score is sum of member scores
    this.teamScore[rootU] += this.teamScore[rootV];
    this.teamMembers[rootU].push(...this.teamMembers[rootV]);
    delete this.teamMembers[rootV];
    return true;
  }

  updateUserScore(userId, delta) {
    if (this.parent[userId] === undefined) {
      this.makeSet(userId, "", delta);
      return;
    }
    this.userScore[userId] += delta;
    const root = this.find(userId);
    if (root !== -1) {
      this.teamScore[root] += delta;
    }
  }

  getTeamScore(userId) {
    const root = this.find(userId);
    return root !== -1 ? this.teamScore[root] : 0;
  }

  getTeamMembers(userId) {
    const root = this.find(userId);
    return root !== -1 ? this.teamMembers[root] || [] : [];
  }

  getTeamName(userId) {
    const root = this.find(userId);
    return root !== -1 ? this.teamName[root] : "Unknown";
  }

  getAllTeams() {
    return Object.keys(this.teamMembers).map(rootId => {
      const id = parseInt(rootId);
      return {
        rootId: id,
        name: this.teamName[id],
        totalScore: this.teamScore[id],
        members: this.teamMembers[id]
      };
    });
  }
}

// Freeze & Reveal Engine
class FreezeRevealEngine {
  constructor(freezeThreshold = 240) {
    this.isFrozen = false;
    this.freezeThreshold = freezeThreshold;
    this.pendingQueue = [];
    this.publicScores = {};
    this.realScores = {};
    this.pendingCounts = {};
    this.nextUpdateId = 1;
  }

  registerUser(userId, initialScore) {
    if (this.realScores[userId] === undefined) {
      this.realScores[userId] = initialScore;
      this.publicScores[userId] = initialScore;
      this.pendingCounts[userId] = 0;
    }
  }

  processJudgedSubmission(userId, userName, problemId, problemName, delta, time, verdict) {
    this.registerUser(userId, 0);
    const oldPub = this.publicScores[userId];
    this.realScores[userId] += delta;

    const frozenNow = this.isFrozen || time >= this.freezeThreshold;
    const update = {
      updateId: this.nextUpdateId++,
      userId,
      userName,
      problemId,
      problemName,
      deltaScore: delta,
      submissionTime: time,
      verdict,
      oldPublicScore: oldPub,
      newPublicScore: oldPub + delta,
      isFrozen: frozenNow
    };

    if (frozenNow) {
      this.pendingCounts[userId]++;
      this.pendingQueue.push(update);
    } else {
      this.publicScores[userId] += delta;
    }
    return update;
  }

  revealNext() {
    if (this.pendingQueue.length === 0) return null;
    const item = this.pendingQueue.shift();
    item.oldPublicScore = this.publicScores[item.userId];
    this.publicScores[item.userId] += item.deltaScore;
    item.newPublicScore = this.publicScores[item.userId];
    if (this.pendingCounts[item.userId] > 0) this.pendingCounts[item.userId]--;
    return item;
  }

  revealAll() {
    const list = [];
    let item;
    while ((item = this.revealNext())) {
      list.push(item);
    }
    return list;
  }
}

// Export for browser
window.Treap = Treap;
window.ScoreSegTree = ScoreSegTree;
window.AVLTree = AVLTree;
window.Trie = Trie;
window.KMP = KMP;
window.BinomialHeap = BinomialHeap;
window.DisjointSetUnion = DisjointSetUnion;
window.FreezeRevealEngine = FreezeRevealEngine;
