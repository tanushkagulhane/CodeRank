// app.js - CodeRank Interactive Frontend Controller
// Connects UI with pure client-side simulation of Member 1, 2, and 3 ADS structures

document.addEventListener("DOMContentLoaded", () => {
  // Global Platform State
  const state = {
    users: [],
    problems: [],
    teams: [],
    submissions: [],
    treap: new window.Treap(),
    segTree: new window.ScoreSegTree(10000),
    avlTree: new window.AVLTree(),
    probTrie: new window.Trie(),
    userTrie: new window.Trie(),
    binomialHeap: new window.BinomialHeap('time'),
    dsu: new window.DisjointSetUnion(),
    freezeEngine: new window.FreezeRevealEngine(240)
  };

  // Toast notification helper
  function showToast(message, type = "info") {
    const container = document.getElementById("toastContainer");
    if (!container) return;
    const toast = document.createElement("div");
    toast.className = `toast toast-${type}`;
    let icon = "⚡";
    if (type === "success") icon = "✅";
    if (type === "error") icon = "⚠️";
    if (type === "freeze") icon = "❄️";
    toast.innerHTML = `<span>${icon}</span> <span>${message}</span>`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.opacity = "0";
      toast.style.transform = "translateX(100%)";
      setTimeout(() => toast.remove(), 300);
    }, 3500);
  }

  // Initialize Platform Data
  async function initPlatform() {
    // Check window.CODERANK_DATA first (works when opened directly via file://)
    let db = window.CODERANK_DATA;
    if (!db) {
      try {
        const res = await fetch("../database/database.json");
        db = await res.json();
      } catch (e) {
        console.warn("Could not load ../database/database.json, using embedded fallback.");
        db = null;
      }
    }

    if (db) {
      state.users = JSON.parse(JSON.stringify(db.users || []));
      state.problems = JSON.parse(JSON.stringify(db.problems || []));
      state.teams = JSON.parse(JSON.stringify(db.teams || []));
      state.submissions = JSON.parse(JSON.stringify(db.submissions || []));
    } else {
      loadEmbeddedFallback();
    }

    // Populate Member 1 ADS
    state.users.forEach(u => {
      state.treap.insert(u.score, u.id, u.username);
      state.segTree.add(u.score, 1);
      state.userTrie.insert(u.username);
    });

    // Populate Member 2 ADS
    state.problems.forEach(p => {
      state.avlTree.insert(p);
      state.probTrie.insert(p.name);
    });

    // Populate Member 3 ADS
    state.users.forEach(u => {
      const teamObj = state.teams.find(t => t.id === u.teamId);
      const teamName = teamObj ? teamObj.name : `Team_${u.teamId || u.id}`;
      state.dsu.makeSet(u.id, teamName, u.score);
      state.freezeEngine.registerUser(u.id, u.score);
    });

    // Union initial team members
    const teamGroups = {};
    state.users.forEach(u => {
      if (u.teamId) {
        if (!teamGroups[u.teamId]) teamGroups[u.teamId] = [];
        teamGroups[u.teamId].push(u.id);
      }
    });

    Object.keys(teamGroups).forEach(tId => {
      const mems = teamGroups[tId];
      if (mems.length > 1) {
        for (let i = 1; i < mems.length; i++) {
          state.dsu.unionSets(mems[0], mems[i]);
        }
      }
    });

    // Populate Submission Queue (ensure time property exists)
    state.submissions.forEach(s => {
      if (s.time === undefined && s.contestTimeMin !== undefined) {
        s.time = s.contestTimeMin;
      }
      state.binomialHeap.insert(s);
    });

    updateHeroStats();
    renderLeaderboard();
    renderProblemset();
    renderQueue();
    renderTeams();
    renderBenchmarkTable();
    populateContestantSelect();
    populatePlagiarismSelect();
    renderVisualizer();
  }

  function loadEmbeddedFallback() {
    state.users = [
      { id: 1, username: "tourist", score: 3800, teamId: 1 },
      { id: 2, username: "Petr", score: 3200, teamId: 1 },
      { id: 3, username: "Benq", score: 3600, teamId: 2 },
      { id: 4, username: "ecnerwala", score: 3500, teamId: 2 },
      { id: 5, username: "Radewoosh", score: 3400, teamId: 3 },
      { id: 6, username: "Um_nik", score: 3300, teamId: 3 },
      { id: 9, username: "tanushka", score: 1850, teamId: 5 },
      { id: 16, username: "nandini", score: 1950, teamId: 5 }
    ];
    state.problems = [
      { id: 1, name: "Watermelon", difficulty: 800, category: "Math" },
      { id: 2, name: "Way Too Long Words", difficulty: 800, category: "Strings" },
      { id: 3, name: "Theatre Square", difficulty: 1000, category: "Math" },
      { id: 4, name: "Next Round", difficulty: 900, category: "Implementation" },
      { id: 29, name: "Dynamic Shortest Path", difficulty: 2800, category: "Graphs" }
    ];
    state.submissions = [
      { id: 1, userId: 1, problemId: 1, time: 15, verdict: "Accepted", code: "cout<<42;" },
      { id: 2, userId: 2, problemId: 1, time: 20, verdict: "Accepted", code: "printf(42);" }
    ];
  }

  function updateHeroStats() {
    document.getElementById("statUsers").innerText = state.users.length;
    document.getElementById("statProblems").innerText = state.problems.length;
    document.getElementById("statQueue").innerText = state.binomialHeap.count;
    document.getElementById("statTeams").innerText = state.dsu.getAllTeams().length;
  }

  // Tab Routing
  const tabs = document.querySelectorAll(".nav-tab");
  tabs.forEach(tab => {
    tab.addEventListener("click", () => {
      tabs.forEach(t => t.classList.remove("active"));
      tab.classList.add("active");
      const targetId = tab.getAttribute("data-tab");
      document.querySelectorAll(".tab-pane").forEach(pane => {
        pane.style.display = pane.id === targetId ? "block" : "none";
      });
      if (targetId === "tab-visualizer") {
        renderVisualizer();
      }
    });
  });

  // -------------------------------------------------------------------
  // TAB 1: LEADERBOARD (MEMBER 1: Treap, Segment Tree, Top-K Heap)
  // -------------------------------------------------------------------
  function renderLeaderboard(filterLo = 0, filterHi = 10000, topK = 0) {
    const tbody = document.getElementById("leaderboardBody");
    if (!tbody) return;
    tbody.innerHTML = "";

    let sortedUsers = state.treap.toSortedArray();
    if (topK > 0) {
      sortedUsers = sortedUsers.slice(0, topK);
    }
    if (filterLo > 0 || filterHi < 10000) {
      sortedUsers = sortedUsers.filter(u => u.score >= filterLo && u.score <= filterHi);
    }

    sortedUsers.forEach((u, index) => {
      const tr = document.createElement("tr");
      let rankBadge = `<span class="rank-badge rank-default">${index + 1}</span>`;
      if (index === 0) rankBadge = `<span class="rank-badge rank-gold">🥇</span>`;
      if (index === 1) rankBadge = `<span class="rank-badge rank-silver">🥈</span>`;
      if (index === 2) rankBadge = `<span class="rank-badge rank-bronze">🥉</span>`;

      let tierBadge = `<span class="tier-badge tier-specialist">Specialist</span>`;
      if (u.score >= 3500) tierBadge = `<span class="tier-badge tier-gm">Legendary GM</span>`;
      else if (u.score >= 3000) tierBadge = `<span class="tier-badge tier-gm">Grandmaster</span>`;
      else if (u.score >= 2100) tierBadge = `<span class="tier-badge tier-master">Master</span>`;
      else if (u.score >= 1600) tierBadge = `<span class="tier-badge tier-expert">Expert</span>`;

      const teamName = state.dsu.getTeamName(u.id);
      const pendingCount = state.freezeEngine.pendingCounts[u.id] || 0;
      const pendingBadge = pendingCount > 0
        ? ` <span class="status-badge frozen" style="font-size:0.7rem; padding:0.15rem 0.45rem; margin-left:0.35rem;">❄️ ${pendingCount} pending</span>`
        : '';

      tr.innerHTML = `
        <td>${rankBadge}</td>
        <td><strong>${u.name}</strong> <small style="color:var(--text-dim)">(ID ${u.id})</small>${pendingBadge}</td>
        <td>${tierBadge}</td>
        <td><strong style="color:var(--accent-purple)">${u.score}</strong></td>
        <td><span style="color:var(--accent-cyan)">${teamName}</span></td>
      `;
      tbody.appendChild(tr);
    });

    // Update Segment tree counter display
    const countInRange = state.segTree.countRange(filterLo, filterHi);
    const maxInRange = state.segTree.maxInRange(filterLo, filterHi);
    const segInfo = document.getElementById("segTreeSummary");
    if (segInfo) {
      segInfo.innerHTML = `Segment Tree Query [${filterLo} - ${filterHi} pts]: <strong>${countInRange}</strong> users | Peak Score: <strong>${maxInRange === -1 ? 'None' : maxInRange}</strong>`;
    }
  }

  // "What's My Rank?" Modal Action
  document.getElementById("btnCheckRank")?.addEventListener("click", () => {
    const handle = prompt("Enter your username to query Treap rank (e.g. tourist, Petr, Benq, tanushka):");
    if (!handle) return;
    const user = state.users.find(u => u.username.toLowerCase() === handle.trim().toLowerCase());
    if (!user) {
      showToast(`User '${handle}' not found in database!`, "error");
      return;
    }
    const r = state.treap.rank(user.score, user.id);
    showToast(`Treap Rank Lookup: '${user.username}' is currently Ranked #${r} with ${user.score} pts!`, "success");
  });

  // Top-K Filter
  document.getElementById("selectTopK")?.addEventListener("change", (e) => {
    const k = parseInt(e.target.value) || 0;
    renderLeaderboard(0, 10000, k);
  });

  // Score Range Filter
  document.getElementById("btnApplyScoreRange")?.addEventListener("click", () => {
    const lo = parseInt(document.getElementById("rangeMinScore")?.value) || 0;
    const hi = parseInt(document.getElementById("rangeMaxScore")?.value) || 10000;
    renderLeaderboard(lo, hi, 0);
  });

  // -------------------------------------------------------------------
  // TAB 2: PROBLEMSET (MEMBER 2: AVL Tree, Trie Autocomplete)
  // -------------------------------------------------------------------
  function renderProblemset(minDiff = 0, maxDiff = 99999) {
    const grid = document.getElementById("problemsGrid");
    if (!grid) return;
    grid.innerHTML = "";

    const list = state.avlTree.listInRange(minDiff, maxDiff);
    list.forEach(p => {
      const card = document.createElement("div");
      card.className = "stat-card";
      card.style.flexDirection = "column";
      card.style.alignItems = "flex-start";

      let diffClass = "diff-easy";
      if (p.difficulty >= 2000) diffClass = "diff-hard";
      else if (p.difficulty >= 1200) diffClass = "diff-med";

      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; width:100%; align-items:center;">
          <span style="color:var(--text-dim); font-size:0.8rem;">#${p.id}</span>
          <span class="diff-badge ${diffClass}">${p.difficulty}</span>
        </div>
        <h4 style="margin: 0.5rem 0 0.25rem 0; font-size:1.05rem;">${p.name}</h4>
        <p style="font-size:0.8rem; color:var(--text-muted);">${p.category || 'Algorithms'}</p>
        <div style="display:flex; gap:0.5rem; width:100%; margin-top:0.75rem;">
          <button class="btn btn-secondary btn-submit-code" data-pid="${p.id}" data-pname="${p.name}" style="flex:1;">
            Submit Code
          </button>
          <button class="btn btn-danger btn-delete-problem" data-pid="${p.id}" data-pname="${p.name}" data-diff="${p.difficulty}" style="padding:0.55rem 0.75rem;">
            🗑️ Delete
          </button>
        </div>
      `;
      grid.appendChild(card);
    });

    document.querySelectorAll(".btn-submit-code").forEach(btn => {
      btn.addEventListener("click", () => {
        const pId = btn.getAttribute("data-pid");
        const pName = btn.getAttribute("data-pname");
        openSubmitModal(pId, pName);
      });
    });

    // Delete problem from AVL Tree, Trie, and state
    document.querySelectorAll(".btn-delete-problem").forEach(btn => {
      btn.addEventListener("click", () => {
        const pId = parseInt(btn.getAttribute("data-pid"));
        const pName = btn.getAttribute("data-pname");
        const pDiff = parseInt(btn.getAttribute("data-diff"));

        state.avlTree.delete(pId, pDiff);
        state.probTrie.delete(pName);
        state.problems = state.problems.filter(p => p.id !== pId);

        showToast(`Problem #${pId} ("${pName}") deleted from AVL Tree!`, "info");
        renderProblemset();
        populatePlagiarismSelect();
        updateHeroStats();
        renderVisualizer();
      });
    });
  }

  // Add Problem form submit
  document.getElementById("btnAddProblem")?.addEventListener("click", () => {
    const nameInput = document.getElementById("newProblemName");
    const diffInput = document.getElementById("newProblemDiff");
    const catInput = document.getElementById("newProblemCategory");

    const name = nameInput?.value?.trim() || "";
    const diff = parseInt(diffInput?.value);
    const cat = catInput?.value?.trim() || "Algorithms";

    // Validate problem inputs
    if (!name) {
      showToast("Problem name cannot be empty!", "error");
      return;
    }
    if (state.problems.some(p => p.name.toLowerCase() === name.toLowerCase())) {
      showToast(`A problem named "${name}" already exists!`, "error");
      return;
    }
    if (isNaN(diff) || diff < 800 || diff > 3500) {
      showToast("Difficulty must be between 800 and 3500!", "error");
      return;
    }

    // Allocate next free problem ID
    const nextId = state.problems.reduce((max, p) => Math.max(max, p.id), 0) + 1;
    const newProblem = {
      id: nextId,
      name: name,
      difficulty: diff,
      category: cat
    };

    state.problems.push(newProblem);
    state.avlTree.insert(newProblem);
    state.probTrie.insert(newProblem.name);

    if (nameInput) nameInput.value = "";
    if (diffInput) diffInput.value = "1200";
    if (catInput) catInput.value = "";

    showToast(`Problem #${newProblem.id} ("${newProblem.name}") added to AVL Tree!`, "success");
    renderProblemset();
    populatePlagiarismSelect();
    updateHeroStats();
    renderVisualizer();
  });

  // Trie Autocomplete on Problems
  const probSearchInput = document.getElementById("searchProblemInput");
  const autoDropdown = document.getElementById("probAutocomplete");
  if (probSearchInput && autoDropdown) {
    probSearchInput.addEventListener("input", (e) => {
      const q = e.target.value.trim();
      if (!q) {
        autoDropdown.style.display = "none";
        renderProblemset();
        return;
      }
      const suggestions = state.probTrie.getAllWithPrefix(q, 6);
      if (suggestions.length === 0) {
        autoDropdown.style.display = "none";
      } else {
        autoDropdown.innerHTML = suggestions.map(s => `<div class="autocomplete-item">${s}</div>`).join("");
        autoDropdown.style.display = "block";
        autoDropdown.querySelectorAll(".autocomplete-item").forEach(item => {
          item.addEventListener("click", () => {
            probSearchInput.value = item.innerText;
            autoDropdown.style.display = "none";
            const match = state.problems.find(p => p.name.toLowerCase() === item.innerText.toLowerCase());
            if (match) renderProblemset(match.difficulty, match.difficulty);
          });
        });
      }
    });
  }

  // Difficulty Range Slider / Filter
  document.getElementById("btnFilterDiff")?.addEventListener("click", () => {
    const minD = parseInt(document.getElementById("minDiffInput")?.value) || 0;
    const maxD = parseInt(document.getElementById("maxDiffInput")?.value) || 99999;
    renderProblemset(minD, maxD);
  });

  // -------------------------------------------------------------------
  // TAB 3: PLAGIARISM CHECKER (MEMBER 2: KMP Algorithm)
  // -------------------------------------------------------------------
  document.getElementById("btnRunPlagiarismScan")?.addEventListener("click", () => {
    const code = document.getElementById("plagiarismCodeInput")?.value || "";
    const pId = parseInt(document.getElementById("plagiarismProbSelect")?.value) || 1;
    const resultBox = document.getElementById("plagiarismResultBox");
    if (!resultBox) return;

    if (!code.trim()) {
      showToast("Please enter or paste code to scan!", "error");
      return;
    }

    const res = window.KMP.checkPlagiarism(code, pId, state.submissions);
    if (res.isPlagiarized) {
      resultBox.style.display = "block";
      resultBox.style.border = "1px solid var(--accent-rose)";
      resultBox.style.background = "rgba(244, 63, 94, 0.1)";
      resultBox.innerHTML = `
        <h4 style="color:var(--accent-rose); display:flex; align-items:center; gap:0.5rem;">
          ⚠️ Plagiarism Detected via KMP Algorithm!
        </h4>
        <p style="margin:0.5rem 0; font-size:0.9rem;">
          Matched with existing database submission from <strong>User #${res.matchedSubmission.userId}</strong> for Problem #${pId}.
        </p>
        <div style="background:#f1f5f9; border:1px solid #e2e8f0; color:#0f172a; padding:0.75rem; border-radius:6px; font-family:'JetBrains Mono',monospace; font-size:0.85rem;">
          Matched Substring Chunk: "${res.matchedChunk}"
        </div>
      `;
      showToast("KMP Scanner: Direct logic match flagged!", "error");
    } else {
      resultBox.style.display = "block";
      resultBox.style.border = "1px solid var(--accent-emerald)";
      resultBox.style.background = "rgba(16, 185, 129, 0.1)";
      resultBox.innerHTML = `
        <h4 style="color:var(--accent-emerald);">✅ Clean Submission Verified</h4>
        <p style="margin:0.5rem 0; font-size:0.9rem;">
          KMP scanned all stored solutions for Problem #${pId}. No matching code windows &ge; 30 characters detected.
        </p>
      `;
      showToast("KMP Scanner: Code is original and clean!", "success");
    }
  });

  // -------------------------------------------------------------------
  // TAB 4: JUDGING & QUEUE (MEMBER 3: Binomial Heap)
  // -------------------------------------------------------------------
  function renderQueue() {
    const listContainer = document.getElementById("queueList");
    if (!listContainer) return;
    listContainer.innerHTML = "";

    const subs = state.binomialHeap.toVector();
    if (subs.length === 0) {
      listContainer.innerHTML = `<p style="color:var(--text-dim); padding:1rem;">(Submission queue is empty)</p>`;
      return;
    }

    subs.forEach(s => {
      const card = document.createElement("div");
      card.className = "tree-node-card";
      const u = state.users.find(x => x.id === s.userId);
      const p = state.problems.find(x => x.id === s.problemId);
      const uName = s.userName || (u ? u.username : `User #${s.userId}`);
      const pName = s.problemName || (p ? p.name : `Problem #${s.problemId}`);
      const sTime = s.time ?? s.contestTimeMin ?? 0;

      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <div>
            <strong>Sub ID #${s.id}</strong> by <strong>${uName}</strong> 
            <span style="color:var(--accent-cyan); margin-left:0.5rem;">${pName}</span>
          </div>
          <span style="font-family:'JetBrains Mono',monospace; color:var(--accent-amber);">${sTime}m</span>
        </div>
      `;
      listContainer.appendChild(card);
    });

    const peek = state.binomialHeap.peekMin();
    const peekBox = document.getElementById("nextJudgedSub");
    if (peekBox) {
      if (peek) {
        const peekU = state.users.find(x => x.id === peek.userId);
        const peekP = state.problems.find(x => x.id === peek.problemId);
        const peekUName = peek.userName || (peekU ? peekU.username : `User #${peek.userId}`);
        const peekPName = peek.problemName || (peekP ? peekP.name : `Problem #${peek.problemId}`);
        const peekTime = peek.time ?? peek.contestTimeMin ?? 0;
        peekBox.innerHTML = `Next in line (Min-Heap Priority): <strong>Sub #${peek.id}</strong> by <strong>${peekUName}</strong> for <em>${peekPName}</em> at <strong>${peekTime}m</strong>`;
      } else {
        peekBox.innerHTML = `Queue is currently empty.`;
      }
    }
  }

  // Helper function to update score everywhere consistently
  function applyScoreChange(userId, delta) {
    const u = state.users.find(x => x.id === userId);
    if (!u) return;
    const oldScore = u.score;
    const newScore = oldScore + delta;

    // Remove old score to prevent duplicate rows in Treap and SegTree
    state.treap.erase(oldScore, u.id);
    state.segTree.add(oldScore, -1);

    // Insert new score
    state.treap.insert(newScore, u.id, u.username);
    state.segTree.add(newScore, 1);

    // Update user object and DSU
    u.score = newScore;
    state.dsu.updateUserScore(userId, delta);
  }

  // Judge Next Button
  document.getElementById("btnJudgeNext")?.addEventListener("click", () => {
    if (state.binomialHeap.count === 0) {
      showToast("Submission queue is empty!", "error");
      return;
    }
    const sub = state.binomialHeap.extractMin();
    const verdict = sub.verdict || "Accepted";
    const delta = (verdict === "Accepted") ? 100 : 0;
    const subTime = sub.time ?? sub.contestTimeMin ?? 0;

    const u = state.users.find(x => x.id === sub.userId);
    const p = state.problems.find(x => x.id === sub.problemId);
    const userName = sub.userName || (u ? u.username : `User_${sub.userId}`);
    const problemName = sub.problemName || (p ? p.name : `Problem_${sub.problemId}`);

    // Process submission in freeze engine (evaluates if frozen by state or submission time)
    const update = state.freezeEngine.processJudgedSubmission(
      sub.userId, userName, sub.problemId, problemName, delta, subTime, verdict
    );

    // If not frozen, apply score change immediately everywhere
    if (!update.isFrozen && delta !== 0) {
      applyScoreChange(sub.userId, delta);
    }

    if (verdict === "Accepted") {
      showToast(`Judged Sub #${sub.id} (${userName} - ${problemName}): Accepted (+100 pts)! ${update.isFrozen ? '[FROZEN]' : ''}`, "success");
    } else {
      showToast(`Judged Sub #${sub.id} (${userName} - ${problemName}): ${verdict} (0 pts). ${update.isFrozen ? '[FROZEN]' : ''}`, "error");
    }

    renderQueue();
    renderLeaderboard();
    renderTeams();
    updateHeroStats();
  });

  // -------------------------------------------------------------------
  // TAB 5: TEAMS (MEMBER 3: Disjoint Set Union)
  // -------------------------------------------------------------------
  function renderTeams() {
    const grid = document.getElementById("teamsGrid");
    if (!grid) return;
    grid.innerHTML = "";

    const allTeams = state.dsu.getAllTeams();
    allTeams.sort((a, b) => b.totalScore - a.totalScore);

    allTeams.forEach((t, i) => {
      const card = document.createElement("div");
      card.className = "stat-card";
      card.style.flexDirection = "column";
      card.style.alignItems = "stretch";

      card.innerHTML = `
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <span class="rank-badge rank-default">#${i + 1}</span>
          <strong style="color:var(--accent-purple); font-size:1.1rem;">${t.totalScore} pts</strong>
        </div>
        <h3 style="margin: 0.5rem 0 0.25rem 0; font-size:1.15rem;">${t.name}</h3>
        <p style="font-size:0.8rem; color:var(--text-muted); margin-bottom:0.6rem;">
          Members (${t.members.length}): ${t.members.map(m => {
            const u = state.users.find(x => x.id === m);
            return u ? u.username : `User #${m}`;
          }).join(", ")}
        </p>
        <span style="font-size:0.75rem; color:var(--accent-cyan);">DSU Root: Node ${t.rootId}</span>
      `;
      grid.appendChild(card);
    });

    // Populate Merge Selectors
    const sel1 = document.getElementById("selectTeam1");
    const sel2 = document.getElementById("selectTeam2");
    if (sel1 && sel2) {
      sel1.innerHTML = allTeams.map(t => `<option value="${t.rootId}">${t.name} (${t.totalScore} pts)</option>`).join("");
      sel2.innerHTML = allTeams.map(t => `<option value="${t.rootId}">${t.name} (${t.totalScore} pts)</option>`).join("");
    }
  }

  // Merge Teams Button (DSU Union by Rank)
  document.getElementById("btnMergeTeams")?.addEventListener("click", () => {
    const t1 = parseInt(document.getElementById("selectTeam1")?.value);
    const t2 = parseInt(document.getElementById("selectTeam2")?.value);
    if (!t1 || !t2 || t1 === t2) {
      showToast("Please select two distinct teams to merge!", "error");
      return;
    }
    const ok = state.dsu.unionSets(t1, t2);
    if (ok) {
      showToast(`Merged Teams via DSU Union by Rank! Team score combines dynamically!`, "success");
      renderTeams();
      renderLeaderboard();
      updateHeroStats();
    } else {
      showToast("Could not merge: already in same team or invalid!", "error");
    }
  });

  // -------------------------------------------------------------------
  // TAB 6: CONTEST FREEZE & REVEAL (MEMBER 3)
  // -------------------------------------------------------------------
  const btnToggleFreeze = document.getElementById("btnToggleFreeze");
  const freezeStatusText = document.getElementById("freezeStatusPill");

  btnToggleFreeze?.addEventListener("click", () => {
    state.freezeEngine.isFrozen = !state.freezeEngine.isFrozen;
    const isF = state.freezeEngine.isFrozen;
    if (isF) {
      btnToggleFreeze.innerText = "Disable Freeze Mode";
      btnToggleFreeze.className = "btn btn-danger";
      if (freezeStatusText) {
        freezeStatusText.innerText = "FROZEN (Last Hour)";
        freezeStatusText.className = "status-badge frozen";
      }
      showToast("Contest Freeze Enabled! Score updates are now withheld from public board.", "freeze");
    } else {
      btnToggleFreeze.innerText = "Enable Freeze Mode";
      btnToggleFreeze.className = "btn btn-secondary";
      if (freezeStatusText) {
        freezeStatusText.innerText = "LIVE CONTEST";
        freezeStatusText.className = "status-badge";
      }
      showToast("Contest Freeze Disabled! Live scores resumed.", "info");
    }
  });

  // Step-by-Step Dramatic Reveal
  document.getElementById("btnRevealNext")?.addEventListener("click", () => {
    const update = state.freezeEngine.revealNext();
    const box = document.getElementById("revealDisplayBox");
    if (!update) {
      showToast("No pending frozen updates remaining to reveal!", "info");
      return;
    }

    // Check old rank before applying score change
    const u = state.users.find(x => x.id === update.userId);
    const oldRank = u ? state.treap.rank(u.score, u.id) : -1;

    // Apply the revealed score change everywhere
    if (update.deltaScore !== 0) {
      applyScoreChange(update.userId, update.deltaScore);
    }

    // Check new rank after applying score change
    const newRank = u ? state.treap.rank(u.score, u.id) : -1;

    if (box) {
      box.style.display = "block";
      box.innerHTML = `
        <h3 style="color:var(--accent-purple); font-size:1.3rem;">🎉 DRAMATIC REVEAL EVENT!</h3>
        <p style="font-size:1.05rem; margin:0.5rem 0;">
          <strong>${update.userName}</strong> solved <strong>${update.problemName}</strong>!
        </p>
        <div style="font-size:1.2rem; font-weight:700; color:var(--accent-emerald);">
          Score: ${update.oldPublicScore} &rarr; ${update.newPublicScore} pts (+${update.deltaScore} pts)
        </div>
        <div style="font-size:1rem; font-weight:600; color:var(--accent-cyan); margin-top:0.4rem;">
          Rank Climb: #${oldRank} &rarr; #${newRank}
        </div>
        <p style="color:var(--text-dim); margin-top:0.4rem;">
          Pending updates remaining in queue: ${state.freezeEngine.pendingQueue.length}
        </p>
      `;
    }
    showToast(`Revealed update for ${update.userName}!`, "success");
    renderLeaderboard();
    renderTeams();
    updateHeroStats();
  });

  // Reveal All Button
  document.getElementById("btnRevealAll")?.addEventListener("click", () => {
    const count = state.freezeEngine.pendingQueue.length;
    if (count === 0) {
      showToast("No pending updates to reveal!", "info");
      return;
    }
    const revealedList = state.freezeEngine.revealAll();
    revealedList.forEach(item => {
      if (item.deltaScore !== 0) {
        applyScoreChange(item.userId, item.deltaScore);
      }
    });

    const box = document.getElementById("revealDisplayBox");
    if (box) {
      box.style.display = "block";
      box.innerHTML = `
        <h3 style="color:var(--accent-emerald); font-size:1.3rem;">🏆 FINAL TRUE STANDINGS UNVEILED!</h3>
        <p>All ${count} withheld updates have been applied in order. Congratulations to the champions!</p>
      `;
    }
    showToast(`Revealed all ${count} pending updates!`, "success");
    renderLeaderboard();
    renderTeams();
    updateHeroStats();
  });

  // -------------------------------------------------------------------
  // TAB 7: BENCHMARK SUITE
  // -------------------------------------------------------------------
  function renderBenchmarkTable() {
    const tbody = document.getElementById("benchmarkBody");
    if (!tbody) return;
    tbody.innerHTML = `
      <tr><td>Queue Insert (2,000 items)</td><td>2,000</td><td style="color:var(--accent-emerald)">0.518 ms</td><td>36.714 ms</td><td><strong style="color:var(--accent-purple)">70.9x</strong></td><td>O(N log N) [Binomial] vs O(N²) [Shift]</td></tr>
      <tr><td>Queue Merge (2 x 5,000 items)</td><td>10,000</td><td style="color:var(--accent-emerald)">0.001 ms</td><td>1.146 ms</td><td><strong style="color:var(--accent-purple)">1000x</strong></td><td>O(log N) [Binomial] vs O(N) [Merge]</td></tr>
      <tr><td>Queue Extract-Min (3,000 items)</td><td>3,000</td><td style="color:var(--accent-emerald)">0.505 ms</td><td>2.076 ms</td><td><strong style="color:var(--accent-purple)">4.1x</strong></td><td>O(N log N) [Heap] vs O(N²) [Search]</td></tr>
      <tr><td>Team Union (8,000 unions)</td><td>8,000</td><td style="color:var(--accent-emerald)">20.007 ms</td><td>26.557 ms</td><td><strong style="color:var(--accent-purple)">1.33x</strong></td><td>O(N α(N)) [DSU] vs O(N²) [Re-label]</td></tr>
      <tr><td>Team Find (50,000 queries)</td><td>50,000</td><td style="color:var(--accent-emerald)">1.324 ms</td><td>71.345 ms</td><td><strong style="color:var(--accent-purple)">53.9x</strong></td><td>O(Q α(N)) [DSU] vs O(Q·D) [Chain]</td></tr>
    `;
  }

  // Live in-browser benchmark runner
  document.getElementById("btnRunBenchmark")?.addEventListener("click", () => {
    const btn = document.getElementById("btnRunBenchmark");
    if (btn) {
      btn.innerText = "⏳ Running Live...";
      btn.disabled = true;
    }

    setTimeout(() => {
      // 1. Treap: 5,000 inserts + 1,000 rank queries vs Sorted Array
      const scores = Array.from({ length: 5000 }, () => Math.floor(Math.random() * 10000));
      const t = new window.Treap();
      const t0 = performance.now();
      for (let i = 0; i < 5000; i++) {
        t.insert(scores[i], i + 1, "User_" + (i + 1));
      }
      for (let i = 0; i < 1000; i++) {
        t.rank(scores[i], i + 1);
      }
      const treapTime = performance.now() - t0;

      const arr = [];
      const p0 = performance.now();
      for (let i = 0; i < 5000; i++) {
        arr.push({ score: scores[i], id: i + 1 });
        arr.sort((a, b) => b.score - a.score || a.id - b.id);
      }
      for (let i = 0; i < 1000; i++) {
        const targetScore = scores[i];
        const targetId = i + 1;
        arr.findIndex(x => x.score === targetScore && x.id === targetId);
      }
      const plainTime1 = performance.now() - p0;
      const speedup1 = (plainTime1 / Math.max(treapTime, 0.01)).toFixed(1);

      // 2. BinomialHeap: 2,000 inserts + extractMin vs Plain Array linear search
      const items = Array.from({ length: 2000 }, (_, i) => ({
        id: i + 1,
        time: Math.floor(Math.random() * 5000),
        problemId: 1
      }));
      const bh = new window.BinomialHeap('time');
      const b0 = performance.now();
      for (let i = 0; i < 2000; i++) {
        bh.insert(items[i]);
      }
      for (let i = 0; i < 2000; i++) {
        bh.extractMin();
      }
      const heapTime = performance.now() - b0;

      const list = [];
      const a0 = performance.now();
      for (let i = 0; i < 2000; i++) {
        list.push(items[i]);
      }
      for (let i = 0; i < 2000; i++) {
        let minIdx = 0;
        for (let j = 1; j < list.length; j++) {
          if (list[j].time < list[minIdx].time) minIdx = j;
        }
        list.splice(minIdx, 1);
      }
      const plainTime2 = performance.now() - a0;
      const speedup2 = (plainTime2 / Math.max(heapTime, 0.01)).toFixed(1);

      // 3. DSU: 20,000 find operations vs Naive parent chain
      const dsu = new window.DisjointSetUnion();
      for (let i = 1; i <= 1000; i++) dsu.makeSet(i);
      for (let i = 1; i < 1000; i++) dsu.unionSets(i, i + 1);

      const d0 = performance.now();
      for (let i = 0; i < 20000; i++) {
        dsu.find((i % 1000) + 1);
      }
      const dsuTime = performance.now() - d0;

      const parent = {};
      for (let i = 1; i <= 1000; i++) parent[i] = i - 1;
      parent[1] = 1;
      function naiveFind(u) {
        while (parent[u] !== u) u = parent[u];
        return u;
      }
      const n0 = performance.now();
      for (let i = 0; i < 20000; i++) {
        naiveFind((i % 1000) + 1);
      }
      const naiveTime = performance.now() - n0;
      const speedup3 = (naiveTime / Math.max(dsuTime, 0.01)).toFixed(1);

      // Update benchmark table
      const tbody = document.getElementById("benchmarkBody");
      if (tbody) {
        tbody.innerHTML = `
          <tr>
            <td>Treap (5k Inserts + 1k Ranks)</td>
            <td>5,000 / 1,000</td>
            <td style="color:var(--accent-emerald)">${treapTime.toFixed(3)} ms</td>
            <td>${plainTime1.toFixed(3)} ms</td>
            <td><strong style="color:var(--accent-purple)">${speedup1}x</strong></td>
            <td>O(log N) vs O(N log N) / O(N)</td>
          </tr>
          <tr>
            <td>Binomial Heap (2k Inserts + 2k ExtractMin)</td>
            <td>2,000</td>
            <td style="color:var(--accent-emerald)">${heapTime.toFixed(3)} ms</td>
            <td>${plainTime2.toFixed(3)} ms</td>
            <td><strong style="color:var(--accent-purple)">${speedup2}x</strong></td>
            <td>O(log N) vs O(N) linear search</td>
          </tr>
          <tr>
            <td>DSU Find Operations (Path Compression)</td>
            <td>20,000</td>
            <td style="color:var(--accent-emerald)">${dsuTime.toFixed(3)} ms</td>
            <td>${naiveTime.toFixed(3)} ms</td>
            <td><strong style="color:var(--accent-purple)">${speedup3}x</strong></td>
            <td>O(α(N)) vs O(N) chain walk</td>
          </tr>
        `;
      }

      const note = document.getElementById("benchmarkNote");
      if (note) {
        note.innerHTML = `<em>Note: Live JavaScript benchmark completed in ${((treapTime + plainTime1 + heapTime + plainTime2 + dsuTime + naiveTime) / 1000).toFixed(2)}s. Initial numbers were C++ results from the backend run.</em>`;
      }

      if (btn) {
        btn.innerText = "⚡ Run Live Benchmark";
        btn.disabled = false;
      }
      showToast("Live benchmark completed!", "success");
    }, 50);
  });

  // -------------------------------------------------------------------
  // TAB 8: ADS VISUALIZER (AVL Tree SVG layout)
  // -------------------------------------------------------------------
  function renderVisualizer() {
    const container = document.getElementById("visualizerSvgContainer");
    if (!container) return;

    const root = state.avlTree.root;
    if (!root) {
      container.innerHTML = `<p style="color:var(--text-dim); padding:2rem;">(AVL Tree is currently empty)</p>`;
      return;
    }

    // In-order traversal to calculate X position and depth for Y position
    let orderIndex = 0;
    let maxDepth = 0;
    const nodeData = new Map();

    function annotate(node, depth = 0) {
      if (!node) return;
      maxDepth = Math.max(maxDepth, depth);
      annotate(node.left, depth + 1);
      orderIndex++;
      nodeData.set(node, { xIndex: orderIndex, depth: depth });
      annotate(node.right, depth + 1);
    }
    annotate(root, 0);

    const xSpacing = 72;
    const ySpacing = 85;
    const paddingX = 40;
    const paddingY = 45;
    const totalWidth = Math.max(850, (orderIndex + 1) * xSpacing + paddingX * 2);
    const totalHeight = (maxDepth + 1) * ySpacing + paddingY * 2;

    let svgLines = "";
    let svgNodes = "";

    function renderNode(node) {
      if (!node) return;
      const data = nodeData.get(node);
      const x = paddingX + data.xIndex * xSpacing;
      const y = paddingY + data.depth * ySpacing;

      // Draw connection lines to children
      if (node.left) {
        const leftData = nodeData.get(node.left);
        const lx = paddingX + leftData.xIndex * xSpacing;
        const ly = paddingY + leftData.depth * ySpacing;
        svgLines += `<line x1="${x}" y1="${y}" x2="${lx}" y2="${ly}" stroke="#cbd5e1" stroke-width="2"/>`;
        renderNode(node.left);
      }
      if (node.right) {
        const rightData = nodeData.get(node.right);
        const rx = paddingX + rightData.xIndex * xSpacing;
        const ry = paddingY + rightData.depth * ySpacing;
        svgLines += `<line x1="${x}" y1="${y}" x2="${rx}" y2="${ry}" stroke="#cbd5e1" stroke-width="2"/>`;
        renderNode(node.right);
      }

      // Balance factor
      const bf = state.avlTree.balance(node);
      let bfColor = "#16a34a"; // 0 is balanced
      if (bf !== 0) bfColor = "#d97706"; // -1 or 1

      const shortName = node.problem.name.length > 11 ? node.problem.name.slice(0, 9) + '..' : node.problem.name;

      svgNodes += `
        <g class="tree-node">
          <circle cx="${x}" cy="${y}" r="22" fill="#ffffff" stroke="#2563eb" stroke-width="2"/>
          <text x="${x}" y="${y + 5}" text-anchor="middle" fill="#0f172a" font-size="11" font-weight="700" font-family="'JetBrains Mono',monospace">${node.problem.difficulty}</text>
          <text x="${x}" y="${y - 28}" text-anchor="middle" fill="#475569" font-size="10" font-weight="500" font-family="'Outfit',sans-serif">#${node.problem.id} ${shortName}</text>
          <text x="${x}" y="${y + 36}" text-anchor="middle" fill="${bfColor}" font-size="10" font-weight="600" font-family="'JetBrains Mono',monospace">BF:${bf}</text>
        </g>
      `;
    }
    renderNode(root);

    container.innerHTML = `
      <svg width="${totalWidth}" height="${totalHeight}" viewBox="0 0 ${totalWidth} ${totalHeight}" style="display:inline-block; max-width:100%;">
        ${svgLines}
        ${svgNodes}
      </svg>
    `;
  }

  document.getElementById("btnRedrawVisualizer")?.addEventListener("click", () => {
    renderVisualizer();
    showToast("AVL Tree visualizer redrawn!", "info");
  });

  // Dynamic dropdown helpers
  function populateContestantSelect() {
    const sel = document.getElementById("modalUserId");
    if (!sel) return;
    sel.innerHTML = state.users.map(u => {
      const teamName = state.dsu.getTeamName(u.id);
      return `<option value="${u.id}">${u.username} (${teamName})</option>`;
    }).join("");
  }

  function populatePlagiarismSelect() {
    const sel = document.getElementById("plagiarismProbSelect");
    if (!sel) return;
    sel.innerHTML = state.problems.map(p => {
      return `<option value="${p.id}">#${p.id} - ${p.name} (Diff: ${p.difficulty})</option>`;
    }).join("");
  }

  // Modal Submit Handlers
  function openSubmitModal(pId, pName) {
    const modal = document.getElementById("submitModal");
    if (!modal) return;
    document.getElementById("modalProblemName").innerText = `#${pId} - ${pName}`;
    document.getElementById("modalProblemId").value = pId;
    populateContestantSelect();
    modal.classList.add("active");
  }

  document.getElementById("btnCloseSubmitModal")?.addEventListener("click", () => {
    document.getElementById("submitModal")?.classList.remove("active");
  });

  document.getElementById("btnConfirmSubmit")?.addEventListener("click", () => {
    const pId = parseInt(document.getElementById("modalProblemId").value);
    const uId = parseInt(document.getElementById("modalUserId").value) || 1;
    const verdict = document.getElementById("modalVerdict")?.value || "Accepted";
    const code = document.getElementById("modalCodeText").value || "cout << 42;";
    const time = parseInt(document.getElementById("modalContestTime").value) || 100;

    // Plagiarism Check
    const plagRes = window.KMP.checkPlagiarism(code, pId, state.submissions);
    if (plagRes.isPlagiarized) {
      showToast(`Submission Rejected: Plagiarism flagged against User #${plagRes.matchedSubmission.userId}!`, "error");
      document.getElementById("submitModal")?.classList.remove("active");
      return;
    }

    const u = state.users.find(x => x.id === uId);
    const p = state.problems.find(x => x.id === pId);

    const sub = {
      id: state.submissions.length + 1,
      userId: uId,
      userName: u ? u.username : `User_${uId}`,
      problemId: pId,
      problemName: p ? p.name : `Problem_${pId}`,
      time: time,
      verdict: verdict,
      code: code
    };
    state.submissions.push(sub);
    state.binomialHeap.insert(sub);

    showToast(`Submission #${sub.id} (${verdict}) queued into Binomial Heap!`, "success");
    document.getElementById("submitModal")?.classList.remove("active");
    renderQueue();
    updateHeroStats();
  });

  // Launch!
  initPlatform();
});
